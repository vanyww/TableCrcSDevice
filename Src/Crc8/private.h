#pragma once

#include "TableCrcSDevice/core_crc8.h"

__SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc8);

struct __SDEVICE_RUNTIME_DATA(TableCrc8)
{
   const uint8_t *LookupTable;
};

__SDEVICE_HANDLE_DECLARATION(TableCrc8);
__SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc8);
