#pragma once

#include "SDeviceCore/config.h"

#if !defined(TABLE_CRC_SDEVICE_USE_EXTERNAL_CONFIG)
   #define TABLE_CRC_SDEVICE_USE_EXTERNAL_CONFIG false
#endif

#if TABLE_CRC_SDEVICE_USE_EXTERNAL_CONFIG
   #include "table_crc_sdevice_config.h"
#endif

#if !defined(TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION)
   #define TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION true
#endif
