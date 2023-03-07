#include "weak.h"

#include "SDeviceCore/errors.h"
#include "SDeviceCore/heap.h"

#include <stdlib.h>

const void *LastUnhandledThrowHandle;
const void *LastLogStatusHandle;
bool AssertFailedCalled;

void SDeviceProcessAssertFail(char *file, int line)
{
   AssertFailedCalled = true;
}

void SDeviceProcessDebugAssertFail(char *file, int line)
{
   AssertFailedCalled = true;
}

void SDeviceProcessUnhandledThrow(const void *handle)
{
   LastUnhandledThrowHandle = handle;
}

void SDeviceProcessLogStatus(const void *handle)
{
   LastLogStatusHandle = handle;
}

void * SDeviceMalloc(size_t size)
{
   return malloc(size);
}

void SDeviceFree(void *_handle)
{
   free(_handle);
}
