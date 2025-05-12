#pragma once

#include "config.h"
#include "dependencies.h"
#include "../public.h"

#include <stdint.h>

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

SDEVICE_CREATE_HANDLE_DECLARATION(TableCrc8, init, context);
SDEVICE_DISPOSE_HANDLE_DECLARATION(TableCrc8, this);

uint8_t TableCrc8SDeviceUpdate(
      SDEVICE_HANDLE(TableCrc8) *this,
      uint8_t                    crc,
      const void                *data,
      size_t                     size);

uint8_t TableCrc8SDeviceCompute(
      SDEVICE_HANDLE(TableCrc8) *this,
      const void                *data,
      size_t                     size);
