#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <limits.h>

#define __LOOKUP_TABLE_LENGTH 256
#define __UINT32_MSB(value) (value & 0x80000000)

static void GenerateCrc32Table(uint32_t polynomial, bool isReverse, uint32_t *lookupTable)
{
   SDeviceDebugAssert(lookupTable != NULL);

   for(size_t byteValue = 0; byteValue < __LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint32_t crc = isReverse ? TableCrcSDeviceInternalReverseUInt32Bits(byteValue) : ((uint32_t)byteValue) << 24;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (__UINT32_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = isReverse ? TableCrcSDeviceInternalReverseUInt32Bits(crc) : crc;
   }
}

static uint32_t UpdateCrc32(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size)
{
   SDeviceDebugAssert(size != 0);
   SDeviceDebugAssert(data != NULL);
   SDeviceDebugAssert(lookupTable != NULL);

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
   SDeviceDebugAssert(size != 0);
   SDeviceDebugAssert(data != NULL);
   SDeviceDebugAssert(lookupTable != NULL);

   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = crc ^ *bytes++;
      crc = (crc >> 8) ^ lookupTable[lookupTableIndex];
   }

   return crc;
}

/**********************************************************************************************************************/

__SDEVICE_STRING_NAME_DEFINITION(TableCrc32);

__SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, _init, _context, _outerNameNode)
{
   SDeviceAssert(_init != NULL);

   const ThisInitData *init = _init;
   ThisHandle *handle = SDeviceMalloc(sizeof(ThisHandle));

   handle->Init = *init;
   handle->Header = (SDeviceHandleHeader)
   {
      .Context = _context,
      .NameNode = { .Name = __SDEVICE_STRING_NAME(TableCrc32), .OuterNode = _outerNameNode },
      .LatestStatus = TABLE_CRC32_SDEVICE_STATUS_OK
   };

   if(init->ExternalLookupTable != NULL)
   {
      handle->Runtime.LookupTable = init->ExternalLookupTable;
   }
   else
   {
      uint32_t *lookupTable = SDeviceMalloc(sizeof(uint32_t) * __LOOKUP_TABLE_LENGTH);
      GenerateCrc32Table(init->Polynomial, init->IsReverse, lookupTable);
      handle->Runtime.LookupTable = lookupTable;
   }

   handle->Runtime.UpdateFunction = (init->IsReverse) ? UpdateReverseCrc32 : UpdateCrc32;

   return handle;
}

__SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, _handlePointer)
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

uint32_t TableCrc32SDeviceUpdate(ThisHandle *handle, uint32_t crc, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return crc;

   SDeviceAssert(data != NULL);

   crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, crc ^ handle->Init.OutputXorValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}

uint32_t TableCrc32SDeviceCompute(ThisHandle *handle, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return handle->Init.InitialValue;

   SDeviceAssert(data != NULL);

   uint32_t crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, handle->Init.InitialValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}
