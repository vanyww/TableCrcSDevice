#include "SDeviceCore/common.h"
#include "Tests/Crc8/test.h"
#include "Tests/Crc16/test.h"
#include "Tests/Crc32/test.h"

#include <stdio.h>
#include <string.h>

struct
{
   bool (*Function)(void);
   char *Description;
} TestFunctions[] =
{
   { TestTableCrc8SDeviceCrc8, "8/CRC-8" },
   { TestTableCrc8SDeviceEbu, "8/EBU" },
   { TestTableCrc8SDeviceItu, "8/ITU" },
   { TestTableCrc16SDeviceCcittFalse, "16/CIITT-FALSE" },
   { TestTableCrc16SDeviceArc, "16/ARC" },
   { TestTableCrc16SDeviceMaxim, "16/MAXIM" },
   { TestTableCrc32SDeviceCrc32, "32/CRC-32" },
   { TestTableCrc32SDeviceBzip2, "32/BZIP2" },
   { TestTableCrc32SDeviceXfer, "32/XFER" },
};

const size_t TestFunctionsCount = LENGTHOF(TestFunctions);

int main(void)
{
   int maxMessageLength = 0;
   for(size_t i = 0; i < TestFunctionsCount; i++)
   {
      int length = strlen(TestFunctions[i].Description);
      if(length > maxMessageLength)
         maxMessageLength = length;
   }

   printf("start tests...\n\n");

   for(size_t i = 0; i < TestFunctionsCount; i++)
      printf("%*s - %s\n",
             -maxMessageLength, TestFunctions[i].Description, TestFunctions[i].Function() == true ? "pass" : "fail");

   printf("\nstop tests...\n");
}
