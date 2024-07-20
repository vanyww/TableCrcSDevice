#pragma once

#include "TableCrcSDevice/Crc32/public.h"

#define IS_VALID_THIS_HANDLE(handle) SDEVICE_IS_VALID_HANDLE(TableCrc32, handle)

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc32);

SDEVICE_RUNTIME_DATA_DECLARATION(TableCrc32)
{
   const uint32_t *LookupTable;

   uint32_t (* UpdateFunction)(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size);
};

SDEVICE_HANDLE_DECLARATION(TableCrc32);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc32);
