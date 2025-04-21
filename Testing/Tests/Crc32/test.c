#include "TableCrcSDevice/Crc32/public.h"

#include "unity_fixture.h"

#define _cleanup __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc32))))

TEST_GROUP(TableCrc32);

TEST_SETUP(TableCrc32) { }
TEST_TEAR_DOWN(TableCrc32) { }

TEST(TableCrc32, Crc32)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true };

   _cleanup SDEVICE_HANDLE(TableCrc32) *handle = SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xCBF43926, crc);
}

TEST(TableCrc32, Bzip2)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false };

   _cleanup SDEVICE_HANDLE(TableCrc32) *handle = SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xFC891918, crc);
}

TEST(TableCrc32, Xfer)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x000000AF, 0x00000000, 0x00000000, false };

   _cleanup SDEVICE_HANDLE(TableCrc32) *handle = SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xBD0BE338, crc);
}

TEST_GROUP_RUNNER(TableCrc32)
{
   RUN_TEST_CASE(TableCrc32, Crc32);
   RUN_TEST_CASE(TableCrc32, Bzip2);
   RUN_TEST_CASE(TableCrc32, Xfer);
}
