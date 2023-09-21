#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/heap.h"

#include <limits.h>

#define UINT8_MSB(value) (value & 0x80)

SDEVICE_IDENTITY_BLOCK_DEFINITION(TableCrc8,
                                  ((const SDeviceUuid)
                                  {
                                     .High = TABLE_CRC8_SDEVICE_UUID_HIGH,
                                     .Low  = TABLE_CRC8_SDEVICE_UUID_LOW
                                  }),
                                  ((const SDeviceVersion)
                                  {
                                     .Major = TABLE_CRC8_SDEVICE_VERSION_MAJOR,
                                     .Minor = TABLE_CRC8_SDEVICE_VERSION_MINOR,
                                     .Patch = TABLE_CRC8_SDEVICE_VERSION_PATCH
                                  }));

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
static void GenerateCrc8Table(uint8_t polynomial, bool isReverse, uint8_t *lookupTable)
{
   for(size_t byteValue = 0; byteValue < TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint8_t crc = (isReverse) ? TableCrcSDeviceInternalReverseUInt8Bits(byteValue) : byteValue;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT8_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = (isReverse) ? TableCrcSDeviceInternalReverseUInt8Bits(crc) : crc;
   }
}
#endif

static uint8_t UpdateCrc8(const uint8_t *lookupTable, uint8_t crc, const void *data, size_t size)
{
   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = crc ^ *bytes++;
      crc = lookupTable[lookupTableIndex];
   }

   return crc;
}

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, owner, identifier, context)
{
   SDeviceAssert(init != NULL);

   const ThisInitData *_init = init;
   ThisHandle *handle = SDeviceAllocHandle(sizeof(ThisInitData), sizeof(ThisRuntimeData));
   handle->Header = (SDeviceHandleHeader)
   {
      .Context       = context,
      .OwnerHandle   = owner,
      .IdentityBlock = &SDEVICE_IDENTITY_BLOCK(TableCrc8),
      .LatestStatus  = TABLE_CRC8_SDEVICE_STATUS_OK,
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
      uint8_t *lookupTable = SDeviceMalloc(sizeof(uint8_t) * TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH);
      GenerateCrc8Table(_init->Polynomial, _init->IsReverse, lookupTable);
      handle->Runtime->LookupTable = lookupTable;
   }
#endif

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, handlePointer)
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

uint8_t TableCrc8SDeviceUpdate(ThisHandle *handle, uint8_t crc, const void *data, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size == 0)
      return crc;

   SDeviceAssert(data != NULL);

   crc = UpdateCrc8(handle->Runtime->LookupTable, crc ^ handle->Init->OutputXorValue, data, size);
   return crc ^ handle->Init->OutputXorValue;
}

uint8_t TableCrc8SDeviceCompute(ThisHandle *handle, const void *data, size_t size)
{
   SDeviceAssert(IS_VALID_THIS_HANDLE(handle));

   if(size == 0)
      return handle->Init->InitialValue;

   SDeviceAssert(data != NULL);

   uint8_t crc = UpdateCrc8(handle->Runtime->LookupTable, handle->Init->InitialValue, data, size);
   return crc ^ handle->Init->OutputXorValue;
}
