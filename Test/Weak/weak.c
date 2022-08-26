#include "weak.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <stdlib.h>

const void *LastUnhandledThrowHandle;
const void *LastLogStatusHandle;
bool AssertFailedCalled;

void _SDeviceProcessAssertFail(char *file, int line)
{
   AssertFailedCalled = true;
}

void _SDeviceProcessUnhandledThrow(const void *_handle)
{
   LastUnhandledThrowHandle = _handle;
}

void _SDeviceLogStatus(const void *_handle)
{
   LastLogStatusHandle = _handle;
}

void * SDeviceMalloc(size_t size)
{
   return malloc(size);
}

void SDeviceFree(void *_handle)
{
   free(_handle);
}
