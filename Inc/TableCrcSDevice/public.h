#pragma once

#include "SDeviceCore/core.h"

#define TABLE_CRC_SDEVICE_VERSION_MAJOR 3
#define TABLE_CRC_SDEVICE_VERSION_MINOR 1
#define TABLE_CRC_SDEVICE_VERSION_PATCH 1
#define TABLE_CRC_SDEVICE_CORE_VERSION ((SDeviceVersion)                                                               \
{                                                                                                                      \
   .Major = TABLE_CRC_SDEVICE_VERSION_MAJOR,                                                                           \
   .Minor = TABLE_CRC_SDEVICE_VERSION_MINOR,                                                                           \
   .Patch = TABLE_CRC_SDEVICE_VERSION_PATCH                                                                            \
})

#define TABLE_CRC_SDEVICE_LOOKUP_TABLE_LENGTH 256
