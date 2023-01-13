#pragma once

#include "TableCrcSDevice/core_crc16.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc16);

struct SDEVICE_RUNTIME_DATA(TableCrc16)
{
   uint16_t (* UpdateFunction)(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size);
   const uint16_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc16);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc16);
