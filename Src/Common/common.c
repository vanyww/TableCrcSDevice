#include "common.h"

#include "SDeviceCore/common.h"

uint32_t TableCrcSDeviceReverseUInt32Bits(uint32_t value)
{
   uint32_t mask;

   mask = 0b11111111111111110000000000000000;
   value = __READ_BITS(value, mask) >> __BIT_SIZEOF(uint32_t) / 2 |
           __READ_BITS(value, ~mask) << __BIT_SIZEOF(uint32_t) / 2;

   mask = 0b11111111000000001111111100000000;
   value = __READ_BITS(value, mask) >> __BIT_SIZEOF(uint32_t) / 4 |
           __READ_BITS(value, ~mask) << __BIT_SIZEOF(uint32_t) / 4;

   mask = 0b11110000111100001111000011110000;
   value = __READ_BITS(value, mask) >> __BIT_SIZEOF(uint32_t) / 8 |
           __READ_BITS(value, ~mask) << __BIT_SIZEOF(uint32_t) / 8;

   mask = 0b11001100110011001100110011001100;
   value = __READ_BITS(value, mask) >> __BIT_SIZEOF(uint32_t) / 16 |
           __READ_BITS(value, ~mask) << __BIT_SIZEOF(uint32_t) / 16;

   mask = 0b10101010101010101010101010101010;
   value = __READ_BITS(value, mask) >> __BIT_SIZEOF(uint32_t) / 32 |
           __READ_BITS(value, ~mask) << __BIT_SIZEOF(uint32_t) / 32;

   return value;
}

uint16_t TableCrcSDeviceReverseUInt16Bits(uint16_t value)
{
   return TableCrcSDeviceReverseUInt32Bits(value) >> (__BIT_SIZEOF(uint32_t) - __BIT_SIZEOF(uint16_t));
}

uint8_t TableCrcSDeviceReverseUInt8Bits(uint8_t value)
{
   return TableCrcSDeviceReverseUInt32Bits(value) >> (__BIT_SIZEOF(uint32_t) - __BIT_SIZEOF(uint8_t));
}
