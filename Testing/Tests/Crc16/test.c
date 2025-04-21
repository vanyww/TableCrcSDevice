#include "TableCrcSDevice/Crc16/public.h"

#include "unity_fixture.h"

#define _cleanup __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc16))))

TEST_GROUP(TableCrc16);

TEST_SETUP(TableCrc16) { }
TEST_TEAR_DOWN(TableCrc16) { }

TEST(TableCrc16, CcittFalse)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x1021, 0xFFFF, 0x0000, false };

   _cleanup SDEVICE_HANDLE(TableCrc16) *handle = SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0x29B1, crc);
}

TEST(TableCrc16, Arc)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x8005, 0x0000, 0x0000, true };

   _cleanup SDEVICE_HANDLE(TableCrc16) *handle = SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0xBB3D, crc);
}

TEST(TableCrc16, Maxim)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x8005, 0x0000, 0xFFFF, true };

   _cleanup SDEVICE_HANDLE(TableCrc16) *handle = SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0x44C2, crc);
}

TEST_GROUP_RUNNER(TableCrc16)
{
   RUN_TEST_CASE(TableCrc16, CcittFalse);
   RUN_TEST_CASE(TableCrc16, Arc);
   RUN_TEST_CASE(TableCrc16, Maxim);
}
