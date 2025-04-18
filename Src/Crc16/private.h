#pragma once

#include "TableCrcSDevice/Crc16/public.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc16);

SDEVICE_RUNTIME_DATA_DECLARATION(TableCrc16)
{
   const uint16_t *LookupTable;

   uint16_t (* UpdateFunction)(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size);
};

SDEVICE_HANDLE_DECLARATION(TableCrc16);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc16);
