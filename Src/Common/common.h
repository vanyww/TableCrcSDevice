#pragma once

#include "TableCrcSDevice/config_base.h"

#include <stdint.h>

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
uint32_t TableCrcSDeviceInternalReverseUInt32Bits(uint32_t value);
uint16_t TableCrcSDeviceInternalReverseUInt16Bits(uint16_t value);
uint8_t TableCrcSDeviceInternalReverseUInt8Bits(uint8_t value);
#endif
