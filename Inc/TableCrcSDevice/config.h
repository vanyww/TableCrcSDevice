#pragma once

#include "SDeviceCore/core.h"

#if (SDEVICE_CORE_VERSION_MAJOR != 8) || (SDEVICE_CORE_VERSION_MINOR < 0)
#error SDevice core version is incorrect.
#endif

#ifdef TABLE_CRC_SDEVICE_USE_EXTERNAL_CONFIG
#include "table_crc_sdevice_config.h"
#endif
