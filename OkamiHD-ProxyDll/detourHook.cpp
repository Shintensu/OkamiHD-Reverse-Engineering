#include "pch.h"

#include "detourHook.h"

bool detourHook(BYTE* hookAddress, void* ourFunct, int length)
{
	if (length < 5)
	{
		return false;
	}

	DWORD oldprotect;
	VirtualProtect(hookAddress, length, PAGE_EXECUTE_READWRITE, &oldprotect);
	//nopping all bytes of the instruction at the hook
	memset(hookAddress, 0x90, length);
	//getting Address of your own function relative to that of the hook
	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)hookAddress) - 5;
	//setting first byte of the hookAddress to jump instruction
	*(BYTE*)hookAddress = 0xE9;
	//setting the 4 bytes after jump instruction to destination
	*(DWORD*)(hookAddress + 1) = relativeAddress;
	VirtualProtect(hookAddress, length, oldprotect, &oldprotect);

	return true;
}