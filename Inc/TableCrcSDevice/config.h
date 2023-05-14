#pragma once

#include "SDeviceCore/core.h"

#if (SDEVICE_CORE_VERSION_MAJOR != 8) || (SDEVICE_CORE_VERSION_MINOR < 0)
#error SDevice core version is incorrect.
#endif

#if defined TABLE_CRC_SDEVICE_USE_EXTERNAL_CONFIG
#include "table_crc_sdevice_config.h"
#endif

#if defined DOXYGEN
#define TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
#undef TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
#endif
