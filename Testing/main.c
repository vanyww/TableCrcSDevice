#include "unity_fixture.h"

static void RunAllTests(void)
{
   RUN_TEST_GROUP(TableCrc8);
   RUN_TEST_GROUP(TableCrc16);
   RUN_TEST_GROUP(TableCrc32);
}

int main(int argc, char const *argv[])
{
   UnityMain(argc, argv, RunAllTests);
   return 0;
}
