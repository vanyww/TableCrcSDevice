#pragma once

#include "SDeviceCore/core.h"

/* version ************************************************************************************************************/

#if (__SDEVICE_CORE_VERSION_MAJOR != 4) || (__SDEVICE_CORE_VERSION_MINOR < 5)
#error SDevice core version is incorrect.
#endif

#define __TABLE_CRC_SDEVICE_VERSION_MAJOR 1
#define __TABLE_CRC_SDEVICE_VERSION_MINOR 3
#define __TABLE_CRC_SDEVICE_VERSION_PATCH 0
#define __TABLE_CRC_SDEVICE_CORE_VERSION ((SDeviceVersion)                                                             \
{                                                                                                                      \
   .Major = __TABLE_CRC_SDEVICE_VERSION_MAJOR,                                                                         \
   .Minor = __TABLE_CRC_SDEVICE_VERSION_MINOR,                                                                         \
   .Patch = __TABLE_CRC_SDEVICE_VERSION_PATCH                                                                          \
})

/**********************************************************************************************************************/
