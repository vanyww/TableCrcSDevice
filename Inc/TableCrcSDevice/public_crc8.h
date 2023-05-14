#pragma once

#include "TableCrcSDevice/public.h"

#include <stdbool.h>

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc8);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc8);

typedef enum
{
   TABLE_CRC8_SDEVICE_STATUS_OK
} TableCrc8SDeviceStatus;

SDEVICE_INIT_DATA_DECLARATION(TableCrc8)
{
   const uint8_t *ExternalLookupTable;
   uint8_t Polynomial;
   uint8_t InitialValue;
   uint8_t OutputXorValue;
   bool IsReverse;
};

SDEVICE_STRING_NAME_DECLARATION(TableCrc8);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, handlePointer);

uint8_t TableCrc8SDeviceUpdate(SDEVICE_HANDLE(TableCrc8) *handle, uint8_t crc, const void *data, size_t size);
uint8_t TableCrc8SDeviceCompute(SDEVICE_HANDLE(TableCrc8) *handle, const void *data, size_t size);
