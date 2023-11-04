#pragma once

#include "TableCrcSDevice/Crc8/public.h"

#include "SDeviceCore/errors.h"

#define IS_VALID_THIS_HANDLE(handle) (                                                                                 \
   {                                                                                                                   \
      ThisHandle *_handle = (handle);                                                                                  \
      _handle != NULL          &&                                                                                      \
      _handle->Init != NULL    &&                                                                                      \
      _handle->Runtime != NULL &&                                                                                      \
      SDeviceCompareIdentityBlocks(SDeviceGetHandleIdentityBlock(_handle),                                             \
                                   &SDEVICE_IDENTITY_BLOCK(TableCrc8));                                                \
   })

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc8);

SDEVICE_RUNTIME_DATA_DECLARATION(TableCrc8)
{
   const uint8_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc8);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc8);
