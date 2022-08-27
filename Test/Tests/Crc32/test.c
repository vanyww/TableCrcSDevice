#include "test.h"

#include "TableCrcSDevice/core_crc32.h"

bool TestTableCrc32SDeviceCrc32(void)
{
   __SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc32)))) __SDEVICE_HANDLE(TableCrc32) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, 0);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xCBF43926)
      return false;

   return true;
}

bool TestTableCrc32SDeviceBzip2(void)
{
   __SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc32)))) __SDEVICE_HANDLE(TableCrc32) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, 0);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xFC891918)
      return false;

   return true;
}

bool TestTableCrc32SDeviceXfer(void)
{
   __SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x000000AF, 0x00000000, 0x00000000, false };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc32)))) __SDEVICE_HANDLE(TableCrc32) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, 0);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xBD0BE338)
      return false;

   return true;
}