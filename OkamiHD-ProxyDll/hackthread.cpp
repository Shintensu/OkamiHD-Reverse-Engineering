#include "pch.h"

#include "proc.h"
#include "mem.h"
#include "detourHook.h"

extern "C" void __fastcall ignoreGravityScript(uintptr_t moduleBase, uintptr_t jmpBackOffset);

void HackThread()
{
    //Create console
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    FILE* pCon;
    freopen_s(&pCon, "CONOUT$", "w", stdout);
    freopen_s(&pCon, "CONIN$", "r", stdin);
    // guided hacking "freopen" > freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Successfully started Hack!" << std::endl;
    //get module base
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"main.dll");
    uintptr_t jmpBackOffset;

    bool bHealth = false, bInk = false, bmovementCheat = false;

    //hack loop
    while (true)
    {
        //getting pointer to player object
        uintptr_t* playerObjectPtr = (uintptr_t*)(moduleBase + 0xB6B2D0);

        //key input
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
            if (bHealth) std::cout << "Health Hack enabled" << std::endl;
            else std::cout << "Health Hack disabled" << std::endl;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bInk = !bInk;
            if (bInk)std::cout << "Ink Hack enabled" << std::endl;
            else std::cout << "Ink Hack disabled" << std::endl;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            //skip company logos
            *(uintptr_t*)(moduleBase + 0xB74414) = 0x00010103;
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            //movement cheats
            bmovementCheat = !bmovementCheat;
            
            

            if (bmovementCheat)
            {
                //changes double jump to be on
                *(uintptr_t*)(moduleBase + 0xB6B2B9) |= 1 << 2;

                //Hold Jump to ignore gravity              
                jmpBackOffset = (moduleBase + 0x3AB596) + 12;
                
                //Ignore Jump Barriers
                mem::Nop((BYTE*)moduleBase + 0x46024B, 7);

                //set max dash speed to very high
                *(float*)(moduleBase + 0x6AF03C) = 13.0f;
            }
            else 
            {
                //compares double Jump ability against Dojo skill entry
                uintptr_t clearUpper = *(uintptr_t*)(moduleBase + 0xB4DFA2) << sizeof(uintptr_t) - 1;   
                *(uintptr_t*)(moduleBase + 0xB6B2B9) &= clearUpper >> sizeof(uintptr_t) - 3;

                //restore original Jump code
                mem::Patch((BYTE*)(moduleBase + 0x3AB596), (BYTE*)"\xF3\x0F\x5C\xC8\xF3\x0F\x58\x8E\x4C\x0E\x00\x00", 12);
                //restore original Jump Barrier code
                mem::Patch((BYTE*)(moduleBase + 0x46024B), (BYTE*)"\x41\x89\x86\x14\x10\x00\x00", 7);

                //restore original dash Speed
                *(float*)(moduleBase + 0x6AF03C) = 6.900000095f;
            }

        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            //noclip
            *(uintptr_t*)(moduleBase + 0xB74414) = 0x00010103;
        }

        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            //teleport
            uintptr_t* playerObjectPtr = (uintptr_t*)(moduleBase + 0xB6B2D0);
            if (*playerObjectPtr)
            {
                float coordUserInputX;
                float coordUserInputY;
                float coordUserInputZ;

                std::cout << "Input X Value" << std::endl;
                std::cin >> coordUserInputX;
                std::cout << "Input Y Value" << std::endl;
                std::cin >> coordUserInputY;
                std::cout << "Input Z Value" << std::endl;
                std::cin >> coordUserInputZ;

                *(BYTE*)(moduleBase + 0xB6B2BB) ^= 1 << 1;
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x0 }) = coordUserInputX;
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x4 }) = coordUserInputY;
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x8 }) = coordUserInputZ;
                *(BYTE*)(moduleBase + 0xB6B2BB) ^= 1 << 1;
            }
        }


        //continous writes
        if (*playerObjectPtr)
        {
            if (bHealth)
            {
                INT16* healthAddress = (INT16*)(moduleBase + 0xB4DF90);
                if (healthAddress)
                {
                    *healthAddress = 1337;
                }
            }

            if (bInk)
            {
                INT16* inkAddress = (INT16*)(moduleBase + 0xB205D9);
                if (inkAddress)
                {
                    *inkAddress = 1337;
                }
            }

            if (bmovementCheat)
            {
                //number of jumps and air tackles used
                *(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1145 }) = 0;
                //number of air tackles used
                *(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1146 }) = 0;
                //Dash Meter
                *(WORD*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0x1174 }) = 351;
            }
        }
    }

    fclose(pCon);
    FreeConsole();
    freopen_s(&pCon, "CONOUT$", "w", stdout);
    FreeConsole();
    

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            HackThread();
        }
    }
}