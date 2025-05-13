#include "TableCrcSDevice/Crc8/public.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(TableCrc8) *Handle;
static SDEVICE_INIT_DATA(TableCrc8) HandleInitArray[] =
{
   { NULL, 0x07, 0x00, 0x00, false },
   { NULL, 0x1D, 0xFF, 0x00, true  },
   { NULL, 0x07, 0x00, 0x55, false }
};

TEST_GROUP(TableCrc8);

TEST_SETUP(TableCrc8)
{
   static unsigned int idx = 0;

   Handle = SDEVICE_CREATE_HANDLE(TableCrc8)(&HandleInitArray[idx], NULL);

   idx++;
}

TEST_TEAR_DOWN(TableCrc8)
{
   SDEVICE_DISPOSE_HANDLE(TableCrc8)(Handle);
}

TEST(TableCrc8, Crc8)
{
   uint8_t crc;

   crc = TableCrc8SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0xF4, crc);
}

TEST(TableCrc8, Ebu)
{
   uint8_t crc;

   crc = TableCrc8SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0x97, crc);
}

TEST(TableCrc8, Itu)
{
   uint8_t crc;

   crc = TableCrc8SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0xA1, crc);
}

TEST_GROUP_RUNNER(TableCrc8)
{
   RUN_TEST_CASE(TableCrc8, Crc8);
   RUN_TEST_CASE(TableCrc8, Ebu);
   RUN_TEST_CASE(TableCrc8, Itu);
}
