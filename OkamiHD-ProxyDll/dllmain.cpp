// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <thread>

#include "util.h"
#include "dinput8Proxy.h"
#include "chainLoad.h"
#include "iniConfig.h"

#include "hackthread.h"

void init();
void setup();


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )

{  
    switch (ul_reason_for_call)
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
        //call init
        init();
        

        break;
    case DLL_PROCESS_DETACH:
        FreeLibrary(hModule);
        break;
    }
    return TRUE;
}

void init() 
{
    std::thread start(setup);
    start.detach();
}

void setup()
{
    HackThread();
}
