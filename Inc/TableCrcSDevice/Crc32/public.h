#pragma once

#include "TableCrcSDevice/public_base.h"

#include "dependencies.h"
#include "config.h"
#include "log.h"

/* 9ed83ee0-58a2-11ee-abd5-a12738c9216a */
#define TABLE_CRC32_SDEVICE_UUID_HIGH 0x9ed83ee058a211ee
#define TABLE_CRC32_SDEVICE_UUID_LOW  0xabd5a12738c9216a

#define TABLE_CRC32_SDEVICE_VERSION_MAJOR TABLE_CRC_SDEVICE_VERSION_MAJOR
#define TABLE_CRC32_SDEVICE_VERSION_MINOR TABLE_CRC_SDEVICE_VERSION_MINOR
#define TABLE_CRC32_SDEVICE_VERSION_PATCH TABLE_CRC_SDEVICE_VERSION_PATCH

SDEVICE_HANDLE_FORWARD_DECLARATION(TableCrc32);
SDEVICE_INIT_DATA_FORWARD_DECLARATION(TableCrc32);

SDEVICE_INIT_DATA_DECLARATION(TableCrc32)
{
   const uint32_t *ExternalLookupTable;

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
   uint32_t Polynomial;
#endif

   uint32_t InitialValue;
   uint32_t OutputXorValue;
   bool IsReverse;
};

SDEVICE_IDENTITY_BLOCK_DECLARATION(TableCrc32);

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc32, init, owner, identifier, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc32, handlePointer);

uint32_t TableCrc32SDeviceUpdate(SDEVICE_HANDLE(TableCrc32) *handle, uint32_t crc, const void *data, size_t size);
uint32_t TableCrc32SDeviceCompute(SDEVICE_HANDLE(TableCrc32) *handle, const void *data, size_t size);