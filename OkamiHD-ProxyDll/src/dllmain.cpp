#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>

#include "util.h"
#include "dinput8Proxy.h"
#include "chainLoad.h"
#include "iniConfig.h"

#include "main.h"

//entry point
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        loadINIConfig();
        //The one real Chainload
        din8DLL = load_real_dinput8();
        chainDLL = chainLoad(chainLoadDLL);
        if (chainDLL != NULL && GetProcAddress(chainDLL, "DirectInput8Create")) {
            DirectInput8Create_fn = (DirectInput8Create_TYPE)GetProcAddress(chainDLL, "DirectInput8Create");
        }
        else {
            DirectInput8Create_fn = (DirectInput8Create_TYPE)GetProcAddress(din8DLL, "DirectInput8Create");
        }

        //create Thread
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, &hinstDLL, 0, NULL);

        break;
    case DLL_PROCESS_DETACH:
        FreeLibrary(hinstDLL);
        break;
    }
    return TRUE;
}