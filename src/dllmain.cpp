#include <windows.h>
#include <thread>
#include "./aimahead/aimahead.h"


void init(HMODULE hModule) {
    std::thread aimahead_gfn(aimahead::initialize, hModule);
    aimahead_gfn.detach();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        init(hModule);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

