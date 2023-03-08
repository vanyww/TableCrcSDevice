#include "test.h"

#include "TableCrcSDevice/public_crc32.h"

bool TestTableCrc32SDeviceCrc32(void)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc32)))) SDEVICE_HANDLE(TableCrc32) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xCBF43926)
      return false;

   return true;
}

bool TestTableCrc32SDeviceBzip2(void)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc32)))) SDEVICE_HANDLE(TableCrc32) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xFC891918)
      return false;

   return true;
}

bool TestTableCrc32SDeviceXfer(void)
{
   SDEVICE_INIT_DATA(TableCrc32) init = { NULL, 0x000000AF, 0x00000000, 0x00000000, false };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc32)))) SDEVICE_HANDLE(TableCrc32) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc32)(&init, NULL, NULL);

   uint32_t crc;

   crc = TableCrc32SDeviceCompute(handle, "12345", 5);
   crc = TableCrc32SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xBD0BE338)
      return false;

   return true;
}
