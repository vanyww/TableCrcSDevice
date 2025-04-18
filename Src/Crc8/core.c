#include "private.h"
#include "../Base/bit_reverse.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/errors.h"

#include <limits.h>

#define UINT8_MSB(value) (value & 0x80)

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc8Table(uint8_t polynomial, bool isReverse, uint8_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint8_t crc = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt8Bits(idx) : idx;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT8_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt8Bits(crc) : crc;
   }
}
#endif

static uint8_t UpdateCrc8(const uint8_t *lookupTable, uint8_t crc, const void *value, size_t size)
{
   for(const char *bytes = value; size > 0; size--, bytes++)
      crc = lookupTable[crc ^ (uint8_t)*bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, context)
{
   SDeviceAssert(init);

   const ThisInitData *_init = init;
   ThisHandle *instance = SDeviceAllocateHandle(sizeof(*instance->Init), sizeof(*instance->Runtime));

   instance->Context = context;
   *instance->Init = *_init;

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
      uint8_t *lookupTable = SDeviceAllocateMemory(sizeof(uint8_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));
      GenerateCrc8Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, handlePointer)
{
   SDeviceAssert(handlePointer);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(handle);

   if(!handle->Init->ExternalLookupTable)
   {
      SDeviceFreeMemory((void *)handle->Runtime->LookupTable);
      handle->Runtime->LookupTable = NULL;
   }

   SDeviceFreeHandle(handle);

   *_handlePointer = NULL;
}

uint8_t TableCrc8SDeviceUpdate(ThisHandle *handle, uint8_t crc, const void *value, size_t size)
{
   SDeviceAssert(handle);

   if(size <= 0)
      return crc;

   SDeviceAssert(value);

   crc = UpdateCrc8(
         handle->Runtime->LookupTable, crc ^ handle->Init->OutputXorValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}

uint8_t TableCrc8SDeviceCompute(ThisHandle *handle, const void *value, size_t size)
{
   SDeviceAssert(handle);

   if(size <= 0)
      return handle->Init->InitialValue;

   SDeviceAssert(value);

   uint8_t crc = UpdateCrc8(
         handle->Runtime->LookupTable, handle->Init->InitialValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}
