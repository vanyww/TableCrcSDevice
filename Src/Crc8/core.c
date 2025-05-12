#include "private.h"
#include "../bit_reverse.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/assert.h"

#include <limits.h>

#define UINT8_MSB(value) (value & 0x80)

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc8Table(
      uint8_t  polynomial,
      bool     isReverse,
      uint8_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint8_t crc =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt8Bits(idx) :
                  idx;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT8_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt8Bits(crc) :
                  crc;
   }
}
#endif

static uint8_t UpdateCrc8(
      const uint8_t *lookupTable,
      uint8_t        crc,
      const void    *data,
      size_t         size)
{
   for(const uint8_t *bytes = data; size > 0; size--, bytes++)
      crc = lookupTable[crc ^ *bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, context)
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
      uint8_t *lookupTable =
            SDeviceAllocateMemory(
                  sizeof(uint8_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));

      GenerateCrc8Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, this)
{
   ThisHandle *_this = this;

   SDeviceAssert(_this);

   if(!_this->Init->ExternalLookupTable)
      SDeviceFreeMemory((void *)_this->Runtime->LookupTable);

   SDeviceFreeHandle(_this);
}

uint8_t TableCrc8SDeviceUpdate(
      ThisHandle *this,
      uint8_t     crc,
      const void *data,
      size_t      size)
{
   SDeviceAssert(this);

   if(size <= 0)
      return crc;

   SDeviceAssert(data);

   crc =
         UpdateCrc8(
               this->Runtime->LookupTable,
               crc ^ this->Init->OutputXorValue,
               data,
               size);

   return crc ^ this->Init->OutputXorValue;
}

uint8_t TableCrc8SDeviceCompute(
      ThisHandle *this,
      const void *data,
      size_t      size)
{
   SDeviceAssert(this);

   if(size <= 0)
      return this->Init->InitialValue;

   SDeviceAssert(data);

   uint8_t crc =
         UpdateCrc8(
               this->Runtime->LookupTable,
               this->Init->InitialValue,
               data,
               size);

   return crc ^ this->Init->OutputXorValue;
}
