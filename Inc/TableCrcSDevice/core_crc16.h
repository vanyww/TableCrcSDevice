#pragma once

#include "core.h"

#include <stdbool.h>

/* SDevice interface **************************************************************************************************/

__SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc16);
__SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc16);

typedef enum
{
   TABLE_CRC16_SDEVICE_STATUS_OK
} TableCrc16SDeviceStatus;

struct __SDEVICE_INIT_DATA(TableCrc16)
{
   const uint16_t *ExternalLookupTable;
   uint16_t Polynomial;
   uint16_t InitialValue;
   uint16_t OutputXorValue;
   bool IsReverse;
};

__SDEVICE_STRING_NAME_DECLARATION(TableCrc16);

__SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, _init, _context, _outerNameNode);
__SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, _handlePointer);

/**********************************************************************************************************************/

uint16_t TableCrc16SDeviceUpdate(__SDEVICE_HANDLE(TableCrc16) *handle, uint16_t crc, const void *data, size_t size);
uint16_t TableCrc16SDeviceCompute(__SDEVICE_HANDLE(TableCrc16) *handle, const void *data, size_t size);
