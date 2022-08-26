#pragma once

#include "TableCrcSDevice/core_crc32.h"

__SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc32);

struct __SDEVICE_RUNTIME_DATA(TableCrc32)
{
   uint32_t (* UpdateFunction)(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size);
   const uint32_t *LookupTable;
};

__SDEVICE_HANDLE_DEFINITION(TableCrc32);
