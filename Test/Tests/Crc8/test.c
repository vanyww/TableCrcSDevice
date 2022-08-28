#include "test.h"

#include "TableCrcSDevice/core_crc8.h"

bool TestTableCrc8SDeviceCrc8(void)
{
   __SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x07, 0x00, 0x00, false };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc8)))) __SDEVICE_HANDLE(TableCrc8) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xF4)
      return false;

   return true;
}

bool TestTableCrc8SDeviceEbu(void)
{
   __SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x1D, 0xFF, 0x00, true };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc8)))) __SDEVICE_HANDLE(TableCrc8) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0x97)
      return false;

   return true;
}

bool TestTableCrc8SDeviceItu(void)
{
   __SDEVICE_INIT_DATA(TableCrc8) init = { NULL, 0x07, 0x00, 0x55, false };
   __attribute__((cleanup(__SDEVICE_DISPOSE_HANDLE(TableCrc8)))) __SDEVICE_HANDLE(TableCrc8) *handle =
            __SDEVICE_CREATE_HANDLE(TableCrc8)(&init, NULL, NULL);

   uint8_t crc;

   crc = TableCrc8SDeviceCompute(handle, "12345", 5);
   crc = TableCrc8SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xA1)
      return false;

   return true;
}
