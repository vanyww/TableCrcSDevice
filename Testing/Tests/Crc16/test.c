#include "test.h"

#include "TableCrcSDevice/Crc16/public.h"

bool TestTableCrc16SDeviceCcittFalse(void)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x1021, 0xFFFF, 0x0000, false };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc16)))) SDEVICE_HANDLE(TableCrc16) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL, 0, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0x29B1)
      return false;

   return true;
}

bool TestTableCrc16SDeviceArc(void)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x8005, 0x0000, 0x0000, true };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc16)))) SDEVICE_HANDLE(TableCrc16) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL, 0, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0xBB3D)
      return false;

   return true;
}

bool TestTableCrc16SDeviceMaxim(void)
{
   SDEVICE_INIT_DATA(TableCrc16) init = { NULL, 0x8005, 0x0000, 0xFFFF, true };
   __attribute__((cleanup(SDEVICE_DISPOSE_HANDLE(TableCrc16)))) SDEVICE_HANDLE(TableCrc16) *handle =
            SDEVICE_CREATE_HANDLE(TableCrc16)(&init, NULL, 0, NULL);

   uint16_t crc;

   crc = TableCrc16SDeviceCompute(handle, "12345", 5);
   crc = TableCrc16SDeviceUpdate(handle, crc, "6789", 4);

   if(crc != 0x44C2)
      return false;

   return true;
}
