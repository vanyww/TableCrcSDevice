#pragma once

#include "TableCrcSDevice/core_crc16.h"

__SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc16);

struct __SDEVICE_RUNTIME_DATA(TableCrc16)
{
   uint16_t (* UpdateFunction)(const uint16_t *lookupTable, uint16_t crc, const void *data, size_t size);
   const uint16_t *LookupTable;
};

__SDEVICE_HANDLE_DECLARATION(TableCrc16);
