#include "TableCrcSDevice/Crc32/public.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(TableCrc32) *Handle;
static SDEVICE_INIT_DATA(TableCrc32) HandleInitArray[] =
{
   { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true  },
   { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false },
   { NULL, 0x000000AF, 0x00000000, 0x00000000, false }
};

TEST_GROUP(TableCrc32);

TEST_SETUP(TableCrc32)
{
   static unsigned int idx = 0;

   Handle = SDEVICE_CREATE_HANDLE(TableCrc32)(&HandleInitArray[idx], NULL);

   idx++;
}

TEST_TEAR_DOWN(TableCrc32)
{
   SDEVICE_DISPOSE_HANDLE(TableCrc32)(Handle);
}

TEST(TableCrc32, Crc32)
{
   uint32_t crc;

   crc = TableCrc32SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xCBF43926, crc);
}

TEST(TableCrc32, Bzip2)
{
   uint32_t crc;

   crc = TableCrc32SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xFC891918, crc);
}

TEST(TableCrc32, Xfer)
{
   uint32_t crc;

   crc = TableCrc32SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT32(0xBD0BE338, crc);
}

TEST_GROUP_RUNNER(TableCrc32)
{
   RUN_TEST_CASE(TableCrc32, Crc32);
   RUN_TEST_CASE(TableCrc32, Bzip2);
   RUN_TEST_CASE(TableCrc32, Xfer);
}
