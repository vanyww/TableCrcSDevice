#pragma once

#include "SDeviceCore/core.h"

/* version ************************************************************************************************************/

#if (SDEVICE_CORE_VERSION_MAJOR != 7) || (SDEVICE_CORE_VERSION_MINOR < 0)
#error SDevice core version is incorrect.
#endif

#define TABLE_CRC_SDEVICE_VERSION_MAJOR 2
#define TABLE_CRC_SDEVICE_VERSION_MINOR 0
#define TABLE_CRC_SDEVICE_VERSION_PATCH 0
#define TABLE_CRC_SDEVICE_CORE_VERSION ((SDeviceVersion)                                                               \
{                                                                                                                      \
   .Major = TABLE_CRC_SDEVICE_VERSION_MAJOR,                                                                           \
   .Minor = TABLE_CRC_SDEVICE_VERSION_MINOR,                                                                           \
   .Patch = TABLE_CRC_SDEVICE_VERSION_PATCH                                                                            \
})

/**********************************************************************************************************************/
