#include "common.h"

#include <limits.h>

uint32_t TableCrcSDeviceReverseUInt32Bits(uint32_t value)
{
   uint32_t mask;

   mask = 0b11111111111111110000000000000000;
   value = (value & mask) >> 16 | (value & ~mask) << 16;

   mask = 0b11111111000000001111111100000000;
   value = (value & mask) >> 8 | (value & ~mask) << 8;

   mask = 0b11110000111100001111000011110000;
   value = (value & mask) >> 4 | (value & ~mask) << 4;

   mask = 0b11001100110011001100110011001100;
   value = (value & mask) >> 2 | (value & ~mask) << 2;

   mask = 0b10101010101010101010101010101010;
   value = (value & mask) >> 1 | (value & ~mask) << 1;

   return value;
}

uint16_t TableCrcSDeviceReverseUInt16Bits(uint16_t value)
{
   return TableCrcSDeviceReverseUInt32Bits(value) >> CHAR_BIT * (sizeof(uint32_t) - sizeof(uint16_t));
}

uint8_t TableCrcSDeviceReverseUInt8Bits(uint8_t value)
{
   return TableCrcSDeviceReverseUInt32Bits(value) >> CHAR_BIT * (sizeof(uint32_t) - sizeof(uint8_t));
}
