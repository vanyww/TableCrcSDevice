#include "private.h"
#include "../Common/common.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <limits.h>

#define __LOOKUP_TABLE_LENGTH 256
#define __UINT16_MSB(value) (value & 0x8000)

static void GenerateCrc16Table(uint16_t polynomial, bool isReverse, uint16_t *lookupTable)
{
   for(size_t byteValue = 0; byteValue < __LOOKUP_TABLE_LENGTH; byteValue++)
   {
      uint16_t crc = (isReverse == true) ? ReverseUInt16Bits(byteValue) : ((uint16_t)byteValue) << 8;

      for(size_t bit = 0; bit < CHAR_BIT; bit++)
         crc = (__UINT16_MSB(crc) != 0) ? (crc << 1) ^ polynomial : crc << 1;

      lookupTable[byteValue] = (isReverse == true) ? ReverseUInt16Bits(crc) : crc;
   }
}

static uint16_t UpdateCrc16(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size)
{
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
   const uint8_t *bytes = data;

   for(; size > 0; size--)
   {
      uint8_t lookupTableIndex = crc ^ *bytes++;
      crc = (crc >> 8) ^ lookupTable[lookupTableIndex];
   }

   return crc;
}

/**********************************************************************************************************************/

__SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, _init, _context, index)
{
   __SDEVICE_INIT_DATA(TableCrc16) *init = _init;

   SDeviceAssert(init != NULL);

   __SDEVICE_HANDLE(TableCrc16) *handle = SDeviceMalloc(sizeof(__SDEVICE_HANDLE(TableCrc16)));

   handle->Header = (SDeviceHandleHeader){ _context, TABLE_CRC16_SDEVICE_STATUS_OK, index };
   handle->Init = *init;

   if(init->ExternalLookupTable != NULL)
   {
      handle->Runtime.LookupTable = init->ExternalLookupTable;
   }
   else
   {
      uint16_t *lookupTable = SDeviceMalloc(sizeof(uint16_t) * __LOOKUP_TABLE_LENGTH);
      GenerateCrc16Table(init->Polynomial, init->IsReverse, lookupTable);
      handle->Runtime.LookupTable = lookupTable;
   }

   handle->Runtime.UpdateFunction = (init->IsReverse) ? UpdateReverseCrc16 : UpdateCrc16;

   return handle;
}

__SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, _handlePointer)
{
   __SDEVICE_HANDLE(TableCrc16) **handlePointer = _handlePointer;
   __SDEVICE_HANDLE(TableCrc16) *handle = *handlePointer;

   if(handle->Init.ExternalLookupTable == NULL)
      SDeviceFree((void *)handle->Runtime.LookupTable);

   SDeviceFree(*handlePointer);
   *handlePointer = NULL;
}

/**********************************************************************************************************************/

uint16_t TableCrc16SDeviceUpdate(__SDEVICE_HANDLE(TableCrc16) *handle, uint16_t crc, const void *data, size_t size)
{
   crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, crc ^ handle->Init.OutputXorValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}

uint16_t TableCrc16SDeviceCompute(__SDEVICE_HANDLE(TableCrc16) *handle, const void *data, size_t size)
{
   uint16_t crc = handle->Runtime.UpdateFunction(handle->Runtime.LookupTable, handle->Init.InitialValue, data, size);
   return crc ^ handle->Init.OutputXorValue;
}