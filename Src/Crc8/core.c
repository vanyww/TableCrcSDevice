#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <limits.h>

#define LOOKUP_TABLE_LENGTH 256
#define UINT8_MSB(value) (value & 0x80)

static void GenerateCrc8Table(uint8_t polynomial, bool isReverse, uint8_t *lookupTable)
{
   SDeviceDebugAssert(lookupTable != NULL);

   for(size_t byteValue = 0; byteValue < LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint8_t crc = isReverse ? TableCrcSDeviceInternalReverseUInt8Bits(byteValue) : byteValue;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT8_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = isReverse ? TableCrcSDeviceInternalReverseUInt8Bits(crc) : crc;
   }
}

static uint8_t UpdateCrc8(const uint8_t *lookupTable, uint8_t crc, const void *data, size_t size)
{
   SDeviceDebugAssert(size != 0);
   SDeviceDebugAssert(data != NULL);
   SDeviceDebugAssert(lookupTable != NULL);

   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = crc ^ *bytes++;
      crc = lookupTable[lookupTableIndex];
   }

   return crc;
}

/**********************************************************************************************************************/

SDEVICE_STRING_NAME_DEFINITION(TableCrc8);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, _init, _context, _outerNameNode)
{
   SDeviceAssert(_init != NULL);

   const ThisInitData *init = _init;
   ThisHandle *handle = SDeviceMalloc(sizeof(ThisHandle));

   handle->Init = *init;
   handle->Header = (SDeviceHandleHeader)
   {
      .Context = _context,
      .NameNode = { .Name = SDEVICE_STRING_NAME(TableCrc8), .OuterNode = _outerNameNode },
      .LatestStatus = TABLE_CRC8_SDEVICE_STATUS_OK
   };

   if(init->ExternalLookupTable != NULL)
   {
      handle->Runtime.LookupTable = init->ExternalLookupTable;
   }
   else
   {
      uint8_t *lookupTable = SDeviceMalloc(sizeof(uint8_t) * LOOKUP_TABLE_LENGTH);
      GenerateCrc8Table(init->Polynomial, init->IsReverse, lookupTable);
      handle->Runtime.LookupTable = lookupTable;
   }

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, _handlePointer)
{
   SDeviceAssert(_handlePointer != NULL);

   ThisHandle **handlePointer = _handlePointer;
   ThisHandle *handle = *handlePointer;

   SDeviceAssert(handle != NULL);

   if(handle->Init.ExternalLookupTable == NULL)
      SDeviceFree((void *)handle->Runtime.LookupTable);

   SDeviceFree(*handlePointer);
   *handlePointer = NULL;
}

/**********************************************************************************************************************/

uint8_t TableCrc8SDeviceUpdate(ThisHandle *handle, uint8_t crc, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return crc;

   SDeviceAssert(data != NULL);

   crc = UpdateCrc8(handle->Runtime.LookupTable, crc ^ handle->Init.OutputXorValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}

uint8_t TableCrc8SDeviceCompute(ThisHandle *handle, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return handle->Init.InitialValue;

   SDeviceAssert(data != NULL);

   uint8_t crc = UpdateCrc8(handle->Runtime.LookupTable, handle->Init.InitialValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}
