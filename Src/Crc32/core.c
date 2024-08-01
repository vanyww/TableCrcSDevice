#include "private.h"
#include "../Base/bit_reverse.h"

#include "SDeviceCore/heap.h"

#include <limits.h>

#define UINT32_MSB(value) (value & 0x80000000)

SDEVICE_IDENTITY_BLOCK_DEFINITION(
      TableCrc32,
      ((const SDeviceUuid)
      {
         .High = TABLE_CRC32_SDEVICE_UUID_HIGH,
         .Low  = TABLE_CRC32_SDEVICE_UUID_LOW
      }),
      ((const SDeviceVersion)
      {
         .Major = TABLE_CRC32_SDEVICE_VERSION_MAJOR,
         .Minor = TABLE_CRC32_SDEVICE_VERSION_MINOR,
         .Patch = TABLE_CRC32_SDEVICE_VERSION_PATCH
      }));

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc32Table(uint32_t polynomial, bool isReverse, uint32_t *lookupTable)
{
   for(size_t idx = 0; idx < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; idx++)
   {
      uint32_t crc = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt32Bits(idx) : ((uint32_t)idx) << 24;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT32_MSB(crc)) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[idx] = (isReverse) ? TableCrcSDeviceBaseInternalReverseUInt32Bits(crc) : crc;
   }
}
#endif

static uint32_t UpdateCrc32(const uint32_t *lookupTable, uint32_t crc, const void *value, size_t size)
{
   for(const char *bytes = value; size > 0; size--, bytes++)
      crc = (crc << CHAR_BIT) ^ lookupTable[(uint8_t)(crc >> 3 * CHAR_BIT) ^ (uint8_t)*bytes];

   return crc;
}

static uint32_t UpdateReverseCrc32(const uint32_t *lookupTable, uint32_t crc, const void *value, size_t size)
{
   for(const char *bytes = value; size > 0; size--, bytes++)
      crc = (crc >> CHAR_BIT) ^ lookupTable[(uint8_t)crc ^ (uint8_t)*bytes];

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, init, owner, identifier, context)
{
   SDeviceAssert(init);

   const ThisInitData *_init = init;
   ThisHandle *instance = SDeviceAllocateHandle(sizeof(*instance->Init), sizeof(*instance->Runtime));

   instance->Header = (SDeviceHandleHeader)
   {
      .Context       = context,
      .OwnerHandle   = owner,
      .IdentityBlock = &SDEVICE_IDENTITY_BLOCK(TableCrc32),
      .LatestStatus  = TABLE_CRC32_SDEVICE_STATUS_OK,
      .Identifier    = identifier
   };

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
      uint32_t *lookupTable = SDeviceAllocateMemory(sizeof(uint32_t[TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH]));
      GenerateCrc32Table(_init->Polynomial, _init->IsReverse, lookupTable);
      instance->Runtime->LookupTable = lookupTable;
   }
#endif

   instance->Runtime->UpdateFunction = (_init->IsReverse) ? UpdateReverseCrc32 : UpdateCrc32;

   return instance;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, handlePointer)
{
   SDeviceAssert(handlePointer);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(!handle->Init->ExternalLookupTable)
   {
      SDeviceFreeMemory((void *)handle->Runtime->LookupTable);
      handle->Runtime->LookupTable = NULL;
   }

   SDeviceFreeHandle(handle);

   *_handlePointer = NULL;
}

uint32_t TableCrc32SDeviceUpdate(ThisHandle *handle, uint32_t crc, const void *value, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size <= 0)
      return crc;

   SDeviceAssert(value);

   crc = handle->Runtime->UpdateFunction(
         handle->Runtime->LookupTable, crc ^ handle->Init->OutputXorValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}

uint32_t TableCrc32SDeviceCompute(ThisHandle *handle, const void *value, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size <= 0)
      return handle->Init->InitialValue;

   SDeviceAssert(value);

   uint32_t crc = handle->Runtime->UpdateFunction(
         handle->Runtime->LookupTable, handle->Init->InitialValue, value, size);

   return crc ^ handle->Init->OutputXorValue;
}
