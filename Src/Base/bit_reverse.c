#include "bit_reverse.h"

#include "SDeviceCore/common.h"

#if TABLE_CRC_SDEVICE_ALLOW_TABLE_GENERATION
uint32_t TableCrcSDeviceBaseInternalReverseUInt32Bits(uint32_t value)
{
   uint32_t mask;

   mask = 0b11111111111111110000000000000000;
   value = READ_BITS(value, mask)  >> BIT_SIZEOF(uint32_t) / 2 |
           READ_BITS(value, ~mask) << BIT_SIZEOF(uint32_t) / 2;

   mask = 0b11111111000000001111111100000000;
   value = READ_BITS(value, mask)  >> BIT_SIZEOF(uint32_t) / 4 |
           READ_BITS(value, ~mask) << BIT_SIZEOF(uint32_t) / 4;

   mask = 0b11110000111100001111000011110000;
   value = READ_BITS(value, mask)  >> BIT_SIZEOF(uint32_t) / 8 |
           READ_BITS(value, ~mask) << BIT_SIZEOF(uint32_t) / 8;

   mask = 0b11001100110011001100110011001100;
   value = READ_BITS(value, mask)  >> BIT_SIZEOF(uint32_t) / 16 |
           READ_BITS(value, ~mask) << BIT_SIZEOF(uint32_t) / 16;

   mask = 0b10101010101010101010101010101010;
   value = READ_BITS(value, mask)  >> BIT_SIZEOF(uint32_t) / 32 |
           READ_BITS(value, ~mask) << BIT_SIZEOF(uint32_t) / 32;

   return value;
}

uint16_t TableCrcSDeviceBaseInternalReverseUInt16Bits(uint16_t value)
{
   return TableCrcSDeviceBaseInternalReverseUInt32Bits(value) >> (BIT_SIZEOF(uint32_t) - BIT_SIZEOF(uint16_t));
}

uint8_t TableCrcSDeviceBaseInternalReverseUInt8Bits(uint8_t value)
{
   return TableCrcSDeviceBaseInternalReverseUInt32Bits(value) >> (BIT_SIZEOF(uint32_t) - BIT_SIZEOF(uint8_t));
}
#endif
