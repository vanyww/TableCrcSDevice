#pragma once

#include "SDeviceCore/core.h"

/* version ************************************************************************************************************/

#if (__SDEVICE_CORE_VERSION_MAJOR != 2) || (__SDEVICE_CORE_VERSION_MINOR < 3)
#error SDevice core version is incorrect.
#endif

#define __CRC_SDEVICE_VERSION_MAJOR 1
#define __CRC_SDEVICE_VERSION_MINOR 0
#define __CRC_SDEVICE_VERSION_PATCH 0
#define __CRC_SDEVICE_CORE_VERSION ((SDeviceVersion)                                                                   \
{                                                                                                                      \
   .Major = __CRC_SDEVICE_VERSION_MAJOR,                                                                               \
   .Minor = __CRC_SDEVICE_VERSION_MINOR,                                                                               \
   .Patch = __CRC_SDEVICE_VERSION_PATCH                                                                                \
})

/**********************************************************************************************************************/
