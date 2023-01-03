#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "mem.h"
#include "proc.h"
#include "detourHook.h"

bool bHealth = false, bInk = false, bmovementCheat = false;

bool show_demo_window = true;
bool show_camera_window = true;
bool show_hack_window = true;
ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
float teleport[3];

void Overlay()
{
    //get main.dll module base
    uintptr_t mainModuleBase = (uintptr_t)GetModuleHandle(L"main.dll");

    //get flower_kernel.dll module base
    uintptr_t flowerKernelModuleBase = (uintptr_t)GetModuleHandle(L"flower_kernel.dll");

    //getting pointer to player object
    uintptr_t* playerObjectPtr = (uintptr_t*)(mainModuleBase + 0xB6B2D0);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // Menu
    {
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Camera Window", &show_camera_window);
        ImGui::Checkbox("Hack Window", &show_hack_window);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // Camera Window
    if (show_camera_window)
    {
        ImGui::Begin("Camera Position", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::InputFloat3("Camera Focus", ((float*)(mainModuleBase + 0xB66370)), "%.3f", 0);
        ImGui::InputFloat3("Camera Position", ((float*)(mainModuleBase + 0xB66380)), "%.3f", 0);
        if (ImGui::Button("Close Me"))
            show_camera_window = false;
        ImGui::End();
    }

    // Hack Window
    if (show_hack_window)
    {
        ImGui::Begin("Hacks", &show_hack_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if (*playerObjectPtr)
        {
            ImGui::Checkbox("Health", &bHealth);
            ImGui::Checkbox("Ink", &bInk);

            if (ImGui::Checkbox("Movement Cheats", &bmovementCheat))
            {
                if (bmovementCheat)
                {
                    //changes double jump to be on
                    *(uintptr_t*)(mainModuleBase + 0xB6B2B9) |= 1 << 2;

                    ////Ignore Jump Barriers
                    //mem::Nop((BYTE*)mainModuleBase + 0x46024B, 7);

                    //set max dash speed to very high
                    *(float*)(mainModuleBase + 0x6AF03C) = 13.0f;
                }
                else
                {
                    //compares double Jump ability against Dojo skill entry
                    uintptr_t clearUpper = *(uintptr_t*)(mainModuleBase + 0xB4DFA2) << sizeof(uintptr_t) - 1;
                    *(uintptr_t*)(mainModuleBase + 0xB6B2B9) &= clearUpper >> sizeof(uintptr_t) - 3;

                    ////restore original Jump code
                    //mem::Patch((BYTE*)(mainModuleBase + 0x3AB596), (BYTE*)"\xF3\x0F\x5C\xC8\xF3\x0F\x58\x8E\x4C\x0E\x00\x00", 12);
                    ////restore original Jump Barrier code
                    //mem::Patch((BYTE*)(mainModuleBase + 0x46024B), (BYTE*)"\x41\x89\x86\x14\x10\x00\x00", 7);

                    //restore original dash Speed
                    *(float*)(mainModuleBase + 0x6AF03C) = 6.900000095f;
                }
            }
            ImGui::InputFloat3("Player Position", (float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x0 }), "%.3f", 0);
            ImGui::InputFloat3("Teleport Destination", teleport, "%.3f", 0);
            if (ImGui::Button("Teleport"))
            {
                *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x0 }) = teleport[0];
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x4 }) = teleport[1];
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x8 }) = teleport[2];
                *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
            }
            if (ImGui::Button("No Clip"))
                *(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00010103;

            //continous writes
            if (bHealth)
            {
                INT16* healthAddress = (INT16*)(mainModuleBase + 0xB4DF90);
                if (healthAddress)
                {
                    *healthAddress = 1337;
                }
            }

            if (bInk)
            {
                INT16* inkAddress = (INT16*)(mainModuleBase + 0xB205D9);
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

        if (ImGui::Button("Skip Company Logos"))
            *(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00010103;

        if (ImGui::Button("Close Me"))
            show_hack_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
}