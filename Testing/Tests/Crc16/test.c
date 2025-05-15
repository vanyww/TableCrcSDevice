#include "TableCrcSDevice/Crc16/public.h"

#include "unity_fixture.h"

static SDEVICE_HANDLE(TableCrc16) *Handle;
static SDEVICE_INIT_DATA(TableCrc16) HandleInitArray[] =
{
   { NULL, 0x1021, 0xFFFF, 0x0000, false },
   { NULL, 0x8005, 0x0000, 0x0000, true  },
   { NULL, 0x8005, 0x0000, 0xFFFF, true  }
};

TEST_GROUP(TableCrc16);

TEST_SETUP(TableCrc16)
{
   static unsigned int idx = 0;

   Handle = SDEVICE_CREATE_HANDLE(TableCrc16)(&HandleInitArray[idx], NULL);

   idx++;
}

TEST_TEAR_DOWN(TableCrc16)
{
   SDEVICE_DISPOSE_HANDLE(TableCrc16)(Handle);
}

TEST(TableCrc16, CcittFalse)
{
   uint16_t crc;

   crc = TableCrc16SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0x29B1, crc);
}

TEST(TableCrc16, Arc)
{
   uint16_t crc;

   crc = TableCrc16SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0xBB3D, crc);
}

TEST(TableCrc16, Maxim)
{
   uint16_t crc;

   crc = TableCrc16SDeviceCompute(Handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(Handle, crc, "6789", 4);

   TEST_ASSERT_EQUAL_UINT16(0x44C2, crc);
}

TEST_GROUP_RUNNER(TableCrc16)
{
   RUN_TEST_CASE(TableCrc16, CcittFalse);
   RUN_TEST_CASE(TableCrc16, Arc);
   RUN_TEST_CASE(TableCrc16, Maxim);
}
