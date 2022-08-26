#pragma once

#include "core.h"

#include <stdbool.h>

/* SDevice interface **************************************************************************************************/

__SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc32);
__SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc32);

typedef enum
{
   TABLE_CRC32_SDEVICE_STATUS_OK
} TableCrc32SDeviceStatus;

struct __SDEVICE_INIT_DATA(TableCrc32)
{
   const uint32_t *ExternalLookupTable;
   uint32_t Polynomial;
   uint32_t InitialValue;
   uint32_t OutputXorValue;
   bool IsReverse;
};

__SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, _init, _context, index);
__SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, _handlePointer);

/**********************************************************************************************************************/

uint32_t TableCrc32SDeviceUpdate(__SDEVICE_HANDLE(TableCrc32) *handle, uint32_t crc, const void *data, size_t size);
uint32_t TableCrc32SDeviceCompute(__SDEVICE_HANDLE(TableCrc32) *handle, const void *data, size_t size);
