#pragma once

#include "TableCrcSDevice/public.h"

#include <stdbool.h>

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc16);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc16);

typedef enum
{
   TABLE_CRC16_SDEVICE_STATUS_OK
} TableCrc16SDeviceStatus;

SDEVICE_INIT_DATA_DECLARATION(TableCrc16)
{
   const uint16_t *ExternalLookupTable;
   uint16_t Polynomial;
   uint16_t InitialValue;
   uint16_t OutputXorValue;
   bool IsReverse;
};

SDEVICE_STRING_NAME_DECLARATION(TableCrc16);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, handlePointer);

uint16_t TableCrc16SDeviceUpdate(SDEVICE_HANDLE(TableCrc16) *handle, uint16_t crc, const void *data, size_t size);
uint16_t TableCrc16SDeviceCompute(SDEVICE_HANDLE(TableCrc16) *handle, const void *data, size_t size);
