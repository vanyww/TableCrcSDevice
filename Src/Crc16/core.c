#include "private.h"
#include "../bit_reverse.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/assert.h"

#include <limits.h>

#define UINT16_MSB(value) (value & 0x8000)

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc16Table(
      uint16_t  polynomial,
      bool      isReverse,
      uint16_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint16_t crc =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt16Bits(idx) :
                  ((uint16_t)idx) << 8;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT16_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt16Bits(crc) :
                  crc;
   }
}
#endif

static uint16_t UpdateCrc16(
      const uint16_t *lookupTable,
      uint16_t        crc,
      const void     *data,
      size_t          size)
{
   for(const uint8_t *bytes = data; size > 0; size--, bytes++)
      crc = (crc << 8) ^ lookupTable[(uint8_t)(crc >> CHAR_BIT) ^ *bytes];

   return crc;
}

static uint16_t UpdateReverseCrc16(
      const uint16_t *lookupTable,
      uint16_t        crc,
      const void     *data,
      size_t          size)
{
   for(const uint8_t *bytes = data; size > 0; size--, bytes++)
      crc = (crc >> CHAR_BIT) ^ lookupTable[(uint8_t)crc ^ *bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, init, context)
{
   const ThisInitData *_init = init;

   SDeviceAssert(_init);

   ThisHandle *instance =
         SDeviceAllocateHandle(
               sizeof(*instance->Init),
               sizeof(*instance->Runtime));

   *instance->Init = *_init;

   instance->Context = context;

#if !TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   SDeviceAssert(_init->ExternalLookupTable);

   instance->Runtime->LookupTable = _init->ExternalLookupTable;
#else
   if(_init->ExternalLookupTable)
   {
      instance->Runtime->LookupTable = _init->ExternalLookupTable;
   }
   else
   {
      uint16_t *lookupTable =
            SDeviceAllocateMemory(
                  sizeof(uint16_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));

      GenerateCrc16Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   instance->Runtime->UpdateCrc =
         (_init->IsReverse) ?
               UpdateReverseCrc16 :
               UpdateCrc16;

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, this)
{
   ThisHandle *_this = this;

   SDeviceAssert(_this);

   if(!_this->Init->ExternalLookupTable)
      SDeviceFreeMemory((void *)_this->Runtime->LookupTable);

   SDeviceFreeHandle(_this);
}

uint16_t TableCrc16SDeviceUpdate(
      ThisHandle *this,
      uint16_t    crc,
      const void *data,
      size_t      size)
{
   SDeviceAssert(this);

   if(size <= 0)
      return crc;

   SDeviceAssert(data);

   crc =
         this->Runtime->UpdateCrc(
               this->Runtime->LookupTable,
               crc ^ this->Init->OutputXorValue,
               data,
               size);

   return crc ^ this->Init->OutputXorValue;
}

uint16_t TableCrc16SDeviceCompute(
      ThisHandle *this,
      const void *data,
      size_t      size)
{
   SDeviceAssert(this);

   if(size <= 0)
      return this->Init->InitialValue;

   SDeviceAssert(data);

   uint16_t crc =
         this->Runtime->UpdateCrc(
               this->Runtime->LookupTable,
               this->Init->InitialValue,
               data,
               size);

   return crc ^ this->Init->OutputXorValue;
}
