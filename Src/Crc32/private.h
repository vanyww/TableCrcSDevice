#pragma once

#include "TableCrcSDevice/core_crc32.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc32);

struct SDEVICE_RUNTIME_DATA(TableCrc32)
{
   uint32_t (* UpdateFunction)(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size);
   const uint32_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc32);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc32);
