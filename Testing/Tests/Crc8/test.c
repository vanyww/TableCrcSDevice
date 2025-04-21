#include "TableCrcSDevice/Crc8/public.h"

#include "unity_fixture.h"

#define _cleanup __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc8))))

TEST_GROUP(TableCrc8);

TEST_SETUP(TableCrc8) { }
TEST_TEAR_DOWN(TableCrc8) { }

TEST(TableCrc8, Crc8)
{
   SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x07, 0x00, 0x00, false };

   _cleanup SDEVICE_HANDLE(TableCrc8) *handle = SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0xF4, crc);
}

TEST(TableCrc8, Ebu)
{
   SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x1D, 0xFF, 0x00, true };

   _cleanup SDEVICE_HANDLE(TableCrc8) *handle = SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0x97, crc);
}

TEST(TableCrc8, Itu)
{
   SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x07, 0x00, 0x55, false };

   _cleanup SDEVICE_HANDLE(TableCrc8) *handle = SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT8(0xA1, crc);
}

TEST_GROUP_RUNNER(TableCrc8)
{
   RUN_TEST_CASE(TableCrc8, Crc8);
   RUN_TEST_CASE(TableCrc8, Ebu);
   RUN_TEST_CASE(TableCrc8, Itu);
}
