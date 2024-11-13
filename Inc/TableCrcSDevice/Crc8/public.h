#pragma once

#include "TableCrcSDevice/public_base.h"

#include "config.h"
#include "dependencies.h"
#include "log.h"

/* ADD99862-58A2-11EE-85C2-4D9EF9E11216 */
#define TABLE_CRC8_SDEVICE_UUID_HIGH 0xADD9986258A211EE
#define TABLE_CRC8_SDEVICE_UUID_LOW  0x85C24D9EF9E11216

#define TABLE_CRC8_SDEVICE_VERSION_MAJOR TABLE_CRC_SDEVICE_VERSION_MAJOR
#define TABLE_CRC8_SDEVICE_VERSION_MINOR TABLE_CRC_SDEVICE_VERSION_MINOR
#define TABLE_CRC8_SDEVICE_VERSION_PATCH TABLE_CRC_SDEVICE_VERSION_PATCH

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc8);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc8);

SDEVICE_INIT_DATA_DECLARATION(TableCrc8)
{
   const uint8_t *ExternalLookupTable;

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   uint8_t Polynomial;
#endif

   uint8_t InitialValue;
   uint8_t OutputXorValue;

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   bool IsReverse;
#endif
};

SDEVICE_IDENTITY_BLOCK_DECLARATION(TableCrc8);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, handlePointer);

uint8_t TableCrc8SDeviceUpdate(SDEVICE_HANDLE(TableCrc8) *handle, uint8_t crc, const void *value, size_t size);
uint8_t TableCrc8SDeviceCompute(SDEVICE_HANDLE(TableCrc8) *handle, const void *value, size_t size);
