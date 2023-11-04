#pragma once

#include "TableCrcSDevice/Crc32/public.h"

#include "SDeviceCore/errors.h"

#define IS_VALID_THIS_HANDLE(handle) (                                                                                 \
   {                                                                                                                   \
      ThisHandle *_handle = (handle);                                                                                  \
      _handle != NULL          &&                                                                                      \
      _handle->Init != NULL    &&                                                                                      \
      _handle->Runtime != NULL &&                                                                                      \
      SDeviceCompareIdentityBlocks(SDeviceGetHandleIdentityBlock(_handle),                                             \
                                   &SDEVICE_IDENTITY_BLOCK(TableCrc32));                                               \
   })

SDEVICE_RUNTIME_DATA_FORWARD_DECLARATION(TableCrc32);

SDEVICE_RUNTIME_DATA_DECLARATION(TableCrc32)
{
   uint32_t (* UpdateFunction)(const uint32_t *lookupTable, uint32_t crc, const void *data, size_t size);
   const uint32_t *LookupTable;
};

SDEVICE_HANDLE_DECLARATION(TableCrc32);
SDEVICE_INTERNAL_ALIASES_DECLARATION(TableCrc32);
