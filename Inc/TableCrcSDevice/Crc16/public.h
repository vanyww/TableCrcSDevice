#pragma once

#include "TableCrcSDevice/public_base.h"

#include "dependencies.h"
#include "config.h"
#include "log.h"

/* 5493ec89-587e-11ee-9b7e-794b1a809a09 */
#define TABLE_CRC16_SDEVICE_UUID_HIGH 0x5493ec89587e11ee
#define TABLE_CRC16_SDEVICE_UUID_LOW  0x9b7e794b1a809a09

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

SDEVICE_IDENTITY_BLOCK_DECLARATION(TableCrc16);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc16, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc16, handlePointer);

uint16_t TableCrc16SDeviceUpdate(SDEVICE_HANDLE(TableCrc16) *handle, uint16_t crc, const void *data, size_t size);
uint16_t TableCrc16SDeviceCompute(SDEVICE_HANDLE(TableCrc16) *handle, const void *data, size_t size);
