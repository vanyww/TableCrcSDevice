#pragma once

#include "TableCrcSDevice/public_base.h"

#include <stdint.h>

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
uint32_t $TableCrcSDeviceBaseReverseUInt32Bits(uint32_t value);
uint16_t $TableCrcSDeviceBaseReverseUInt16Bits(uint16_t value);
uint8_t $TableCrcSDeviceBaseReverseUInt8Bits(uint8_t value);
#endif
