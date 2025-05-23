#pragma once

#include "TableCrcSDevice/Crc16/public.h"

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc16);

SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc16);

SDEVICE_RUNTIME_DATA_DECLARATION(TableCrc16)
{
   uint16_t (* UpdateCrc)(
         const uint16_t *lookupTable,
         uint16_t        crc,
         const void     *data,
         size_t          size);

   const uint16_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc16);
