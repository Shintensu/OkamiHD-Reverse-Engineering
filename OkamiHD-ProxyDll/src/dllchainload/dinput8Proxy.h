#pragma once
#include "pch.h"

#include <Windows.h>
#include <Unknwn.h>
#include <dinput.h>

/* dinput8 has no explicit typedef like dx has */
typedef HRESULT(WINAPI* DirectInput8Create_TYPE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DirectInput8Create_TYPE DirectInput8Create_fn = nullptr;

HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    HRESULT ret = DirectInput8Create_fn(hinst, dwVersion, riidltf, ppvOut, punkOuter);
    return ret;
}
