#pragma once

#include "core.h"

#include <stdbool.h>

/* SDevice interface **************************************************************************************************/

__SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc8);
__SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc8);

typedef enum
{
   TABLE_CRC8_SDEVICE_STATUS_OK
} TableCrc8SDeviceStatus;

struct __SDEVICE_INIT_DATA(TableCrc8)
{
   const uint8_t *ExternalLookupTable;
   uint8_t Polynomial;
   uint8_t InitialValue;
   uint8_t OutputXorValue;
   bool IsReverse;
};

__SDEVICE_STRING_NAME_DECLARATION(TableCrc8);

__SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, _init, _context, _outerNameNode);
__SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, _handlePointer);

/**********************************************************************************************************************/

uint8_t TableCrc8SDeviceUpdate(__SDEVICE_HANDLE(TableCrc8) *handle, uint8_t crc, const void *data, size_t size);
uint8_t TableCrc8SDeviceCompute(__SDEVICE_HANDLE(TableCrc8) *handle, const void *data, size_t size);
