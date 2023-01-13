#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <limits.h>

#define LOOKUP_TABLE_LENGTH 256
#define UINT16_MSB(value) (value & 0x8000)

static void GenerateCrc16Table(uint16_t polynomial, bool isReverse, uint16_t *lookupTable)
{
   SDeviceDebugAssert(lookupTable != NULL);

   for(size_t byteValue = 0; byteValue < LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint16_t crc = isReverse ? TableCrcSDeviceInternalReverseUInt16Bits(byteValue) : ((uint16_t)byteValue) << 8;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (UINT16_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = isReverse ? TableCrcSDeviceInternalReverseUInt16Bits(crc) : crc;
   }
}

static uint16_t UpdateCrc16(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size)
{
   SDeviceDebugAssert(size != 0);
   SDeviceDebugAssert(data != NULL);
   SDeviceDebugAssert(lookupTable != NULL);

   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = (crc >> 8) ^ *bytes++;
      crc = (crc << 8) ^ lookupTable[lookupTableIndex];
   }

   return crc;
}

static uint16_t UpdateReverseCrc16(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size)
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

SDEVICE_STRING_NAME_DEFINITION(TableCrc16);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, _init, _context, _outerNameNode)
{
   SDeviceAssert(_init != NULL);

   const ThisInitData *init = _init;
   ThisHandle *handle = SDeviceMalloc(sizeof(ThisHandle));

   handle->Init = *init;
   handle->Header = (SDeviceHandleHeader)
   {
      .Context = _context,
      .NameNode = { .Name = SDEVICE_STRING_NAME(TableCrc16), .OuterNode = _outerNameNode },
      .LatestStatus = TABLE_CRC16_SDEVICE_STATUS_OK
   };

   if(init->ExternalLookupTable != NULL)
   {
      handle->Runtime.LookupTable = init->ExternalLookupTable;
   }
   else
   {
      uint16_t *lookupTable = SDeviceMalloc(sizeof(uint16_t) * LOOKUP_TABLE_LENGTH);
      GenerateCrc16Table(init->Polynomial, init->IsReverse, lookupTable);
      handle->Runtime.LookupTable = lookupTable;
   }

   handle->Runtime.UpdateFunction = (init->IsReverse) ? UpdateReverseCrc16 : UpdateCrc16;

   return handle;
}

SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, _handlePointer)
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

uint16_t TableCrc16SDeviceUpdate(ThisHandle *handle, uint16_t crc, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return crc;

   SDeviceAssert(data != NULL);

   crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, crc ^ handle->Init.OutputXorValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}

uint16_t TableCrc16SDeviceCompute(ThisHandle *handle, const void *data, size_t size)
{
   SDeviceAssert(handle != NULL);

   if(size == 0)
      return handle->Init.InitialValue;

   SDeviceAssert(data != NULL);

   uint16_t crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, handle->Init.InitialValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}
