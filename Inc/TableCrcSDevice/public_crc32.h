#pragma once

#include "TableCrcSDevice/public_base.h"

#include <stdbool.h>

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc32);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc32);

typedef enum
{
   TABLE_CRC32_SDEVICE_STATUS_OK
} TableCrc32SDeviceStatus;

SDEVICE_INIT_DATA_DECLARATION(TableCrc32)
{
   const uint32_t *ExternalLookupTable;

#if defined TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   uint32_t Polynomial;
#endif

   uint32_t InitialValue;
   uint32_t OutputXorValue;
   bool IsReverse;
};

SDEVICE_STRING_NAME_DECLARATION(TableCrc32);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, handlePointer);

uint32_t TableCrc32SDeviceUpdate(SDEVICE_HANDLE(TableCrc32) *handle, uint32_t crc, const void *data, size_t size);
uint32_t TableCrc32SDeviceCompute(SDEVICE_HANDLE(TableCrc32) *handle, const void *data, size_t size);
