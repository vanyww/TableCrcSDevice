#pragma once

#include "TableCrcSDevice/public_base.h"

#include "config.h"
#include "dependencies.h"

#define TABLE_CRC16_SDEVICE_VERSION_MAJOR TABLE_CRC_SDEVICE_VERSION_MAJOR
#define TABLE_CRC16_SDEVICE_VERSION_MINOR TABLE_CRC_SDEVICE_VERSION_MINOR
#define TABLE_CRC16_SDEVICE_VERSION_PATCH TABLE_CRC_SDEVICE_VERSION_PATCH

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc16);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc16);

SDEVICE_INIT_DATA_DECLARATION(TableCrc16)
{
   const uint16_t *ExternalLookupTable;

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   uint16_t Polynomial;
#endif

   uint16_t InitialValue;
   uint16_t OutputXorValue;
   bool IsReverse;
};

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, init, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, handlePointer);

uint16_t TableCrc16SDeviceUpdate(SDEVICE_HANDLE(TableCrc16) *handle, uint16_t crc, const void *value, size_t size);
uint16_t TableCrc16SDeviceCompute(SDEVICE_HANDLE(TableCrc16) *handle, const void *value, size_t size);
