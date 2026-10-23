// Link against the actual normal product objects. This executes the canonical
// Win32 owner, source constructor/destructor and real worker/CRT transport;
// no Supervisor or SDK fixture replaces them. No game entry point is invoked.
#include "SupervisorRuntime.hpp"

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(value) do { if (!(value)) { fprintf(stderr, "lifecycle check line %d: %s\n", __LINE__, #value); abort(); } } while (0)

static void __fastcall CountCompletedWorker(void *argument)
{
    InterlockedIncrement(static_cast<LONG *>(argument));
}

static u32 StorageWord(const unsigned char *storage, size_t offset)
{
    u32 value;
    memcpy(&value, storage + offset, sizeof(value));
    return value;
}

int __cdecl main()
{
    using namespace th095;
    unsigned char *storage = static_cast<unsigned char *>(malloc(sizeof(Supervisor)));
    CHECK(storage != NULL);
    memset(storage, 0xa5, sizeof(Supervisor));
    Supervisor *supervisor = new (storage) Supervisor;

    // The target resets its whole live instance, then sets only bits 6 and 8.
    for (size_t i = 0; i < sizeof(Supervisor); ++i)
    {
        unsigned int expected = 0;
        if (i >= offsetof(Supervisor, flags) && i < offsetof(Supervisor, flags) + 4)
            expected = (0x140u >> ((i - offsetof(Supervisor, flags)) * 8)) & 0xff;
        CHECK(storage[i] == expected);
    }

    // A loaded image assigns through the canonical base without constructing
    // another initialized Supervisor member or changing adjacent storage.
    GameConfiguration image;
    memset(&image, 0x5a, sizeof(image));
    supervisor->config = image;
    CHECK(memcmp(static_cast<GameConfiguration *>(&supervisor->config), &image, sizeof(image)) == 0);
    CHECK(reinterpret_cast<unsigned char *>(static_cast<GameConfiguration *>(&supervisor->config))
          == storage + offsetof(Supervisor, config));
    CHECK(StorageWord(storage, offsetof(Supervisor, flags)) == 0x140);

    LONG completed[2] = {0, 0};
    supervisor->replayScanWorker.Start(CountCompletedWorker, &completed[0]);
    supervisor->secondaryReplayScanWorker.Start(CountCompletedWorker, &completed[1]);
    HANDLE first = reinterpret_cast<HANDLE>(supervisor->replayScanWorker.threadHandle);
    HANDLE second = reinterpret_cast<HANDLE>(supervisor->secondaryReplayScanWorker.threadHandle);
    CHECK(first != NULL && second != NULL);
    supervisor->~Supervisor();
    CHECK(completed[0] == 1 && completed[1] == 1);
    DWORD handleFlags;
    CHECK(!GetHandleInformation(first, &handleFlags) && GetLastError() == ERROR_INVALID_HANDLE);
    CHECK(!GetHandleInformation(second, &handleFlags) && GetLastError() == ERROR_INVALID_HANDLE);
    const size_t workers[] = {offsetof(Supervisor, replayScanWorker), offsetof(Supervisor, secondaryReplayScanWorker)};
    for (unsigned int i = 0; i < 2; ++i)
    {
        CHECK(StorageWord(storage, workers[i] + offsetof(ReplayScanWorker, threadHandle)) == 0);
        CHECK(StorageWord(storage, workers[i] + offsetof(ReplayScanWorker, threadProc)) == 0);
        CHECK(StorageWord(storage, workers[i] + offsetof(ReplayScanWorker, exitSignal)) == 1);
        CHECK(StorageWord(storage, workers[i] + offsetof(ReplayScanWorker, active)) == 0);
    }
    free(storage);
    puts("canonical Supervisor: target reset/flags, base-image assignment and real worker teardown passed");
    return 0;
}
