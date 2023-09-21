#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/heap.h"

#include <limits.h>

#define UINT32_MSB(value) (value & 0x80000000)

SDEVICE_IDENTITY_BLOCK_DEFINITION(TableCrc32,
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
   for(size_t byteValue = 0; byteValue < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint32_t crc = (isReverse) ? TableCrcSDeviceInternalReverseUInt32Bits(byteValue) : ((uint32_t)byteValue) << 24;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT32_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = (isReverse) ? TableCrcSDeviceInternalReverseUInt32Bits(crc) : crc;
   }
}
#endif

static uint32_t UpdateCrc32(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size)
{
   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = (crc >> 24) ^ *bytes++;
      crc = (crc << 8) ^ lookupTable[lookupTableIndex];
   }

   return crc;
}

static uint32_t UpdateReverseCrc32(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size)
{
   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = crc ^ *bytes++;
      crc = (crc >> 8) ^ lookupTable[lookupTableIndex];
   }

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, init, owner, identifier, context)
{
   SDeviceAssert(init != NULL);

   const ThisInitData *_init = init;
   ThisHandle *handle = SDeviceAllocHandle(sizeof(ThisInitData), sizeof(ThisRuntimeData));
   handle->Header = (SDeviceHandleHeader)
   {
      .Context       = context,
      .OwnerHandle   = owner,
      .IdentityBlock = &SDEVICE_IDENTITY_BLOCK(TableCrc32),
      .LatestStatus  = TABLE_CRC32_SDEVICE_STATUS_OK,
      .Identifier    = identifier
   };
   *handle->Init = *_init;

#if !TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   SDeviceAssert(_init->ExternalLookupTable != NULL);

   handle->Runtime->LookupTable = _init->ExternalLookupTable;
#else
   if(_init->ExternalLookupTable != NULL)
   {
      handle->Runtime->LookupTable = _init->ExternalLookupTable;
   }
   else
   {
      uint32_t *lookupTable = SDeviceMalloc(sizeof(uint32_t) * TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH);
      GenerateCrc32Table(_init->Polynomial, _init->IsReverse, lookupTable);
      handle->Runtime->LookupTable = lookupTable;
   }
#endif

   handle->Runtime->UpdateFunction = (_init->IsReverse) ? UpdateReverseCrc32 : UpdateCrc32;

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, handlePointer)
{
   SDeviceAssert(handlePointer != NULL);

   ThisHandle **_handlePointer = handlePointer;
   ThisHandle *handle = *_handlePointer;

   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(handle->Init->ExternalLookupTable == NULL)
      SDeviceFree((void *)handle->Runtime->LookupTable);

   SDeviceFreeHandle(handle);
   *_handlePointer = NULL;
}

uint32_t TableCrc32SDeviceUpdate(ThisHandle *handle, uint32_t crc, const void *data, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size == 0)
      return crc;

   SDeviceAssert(data != NULL);

   crc = handle->Runtime->UpdateFunction(handle->Runtime->LookupTable, crc ^ handle->Init->OutputXorValue, data, size);
   return crc ^ handle->Init->OutputXorValue;
}

uint32_t TableCrc32SDeviceCompute(ThisHandle *handle, const void *data, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size == 0)
      return handle->Init->InitialValue;

   SDeviceAssert(data != NULL);

   uint32_t crc = handle->Runtime->UpdateFunction(handle->Runtime->LookupTable, handle->Init->InitialValue, data, size);
   return crc ^ handle->Init->OutputXorValue;
}
