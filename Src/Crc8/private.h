#pragma once

#include "TableCrcSDevice/core_crc8.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc8);

struct SDEVICE_RUNTIME_DATA(TableCrc8)
{
   const uint8_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc8);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc8);
