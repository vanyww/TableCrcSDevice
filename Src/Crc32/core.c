#include "private.h"
#include "../bit_reverse.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/assert.h"

#include <limits.h>

#define UINT32_MSB(value) (value & 0x80000000)

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc32Table(
      uint32_t  polynomial,
      bool      isReverse,
      uint32_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint32_t crc =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt32Bits(idx) :
                  ((uint32_t)idx) << 24;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT32_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] =
            (isReverse) ?
                  TableCrcSDeviceInternalReverseUInt32Bits(crc) :
                  crc;
   }
}
#endif

static uint32_t UpdateCrc32(
      const uint32_t *lookupTable,
      uint32_t        crc,
      const void     *data,
      size_t          size)
{
   for(const uint8_t *bytes = data; size > 0; size--, bytes++)
      crc = (crc << CHAR_BIT) ^ lookupTable[(uint8_t)(crc >> 3 * CHAR_BIT) ^ *bytes];

   return crc;
}

static uint32_t UpdateReverseCrc32(
      const uint32_t *lookupTable,
      uint32_t        crc,
      const void     *data,
      size_t          size)
{
   for(const uint8_t *bytes = data; size > 0; size--, bytes++)
      crc = (crc >> CHAR_BIT) ^ lookupTable[(uint8_t)crc ^ *bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, init, context)
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
      uint32_t *lookupTable =
            SDeviceAllocateMemory(
                  sizeof(uint32_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));

      GenerateCrc32Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   instance->Runtime->UpdateCrc =
         (_init->IsReverse) ?
               UpdateReverseCrc32 :
               UpdateCrc32;

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, this)
{
   ThisHandle *_this = this;

   SDeviceAssert(_this);

   if(!_this->Init->ExternalLookupTable)
      SDeviceFreeMemory((void *)_this->Runtime->LookupTable);

   SDeviceFreeHandle(_this);
}

uint32_t TableCrc32SDeviceUpdate(
      ThisHandle *this,
      uint32_t    crc,
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

uint32_t TableCrc32SDeviceCompute(
      ThisHandle *this,
      const void *data,
      size_t      size)
{
   SDeviceAssert(this);

   if(size <= 0)
      return this->Init->InitialValue;

   SDeviceAssert(data);

   uint32_t crc =
         this->Runtime->UpdateCrc(
               this->Runtime->LookupTable,
               this->Init->InitialValue,
               data,
               size);

   return crc ^ this->Init->OutputXorValue;
}
