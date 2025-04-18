#include "private.h"
#include "../Base/bit_reverse.h"

#include "SDeviceCore/heap.h"
#include "SDeviceCore/errors.h"

#include <limits.h>

#define UINT16_MSB(value) (value & 0x8000)

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc16Table(uint16_t polynomial, bool isReverse, uint16_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint16_t crc = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt16Bits(idx) : ((uint16_t)idx) << 8;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT16_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt16Bits(crc) : crc;
   }
}
#endif

static uint16_t UpdateCrc16(const uint16_t *lookupTable, uint16_t crc, const void *value, size_t size)
{
   for(const char *bytes = value; size > 0; size--, bytes++)
      crc = (crc << 8) ^ lookupTable[(uint8_t)(crc >> CHAR_BIT) ^ (uint8_t)*bytes];

   return crc;
}

static uint16_t UpdateReverseCrc16(const uint16_t *lookupTable, uint16_t crc, const void *value, size_t size)
{
   for(const char *bytes = value; size > 0; size--, bytes++)
      crc = (crc >> CHAR_BIT) ^ lookupTable[(uint8_t)crc ^ (uint8_t)*bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, init, context)
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
      uint16_t *lookupTable = SDeviceAllocateMemory(sizeof(uint16_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));
      GenerateCrc16Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   instance->Runtime->UpdateFunction = (_init->IsReverse) ? UpdateReverseCrc16 : UpdateCrc16;

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, handlePointer)
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

uint16_t TableCrc16SDeviceUpdate(ThisHandle *handle, uint16_t crc, const void *value, size_t size)
{
   SDeviceAssert(handle);

   if(size <= 0)
      return crc;

   SDeviceAssert(value);

   crc = handle->Runtime->UpdateFunction(
         handle->Runtime->LookupTable, crc ^ handle->Init->OutputXorValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}

uint16_t TableCrc16SDeviceCompute(ThisHandle *handle, const void *value, size_t size)
{
   SDeviceAssert(handle);

   if(size <= 0)
      return handle->Init->InitialValue;

   SDeviceAssert(value);

   uint16_t crc = handle->Runtime->UpdateFunction(
         handle->Runtime->LookupTable, handle->Init->InitialValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}
