#pragma once

#include "pch.h"

#include <Windows.h>
#include <string> 
#include <vector>

static HMODULE chainDLL = nullptr;
static HMODULE din8DLL = nullptr;
std::vector<HMODULE> lazydlls;

HMODULE load_real_dinput8() {
    wchar_t path[MAX_PATH];
    GetSystemDirectory(path, MAX_PATH);
    lstrcatW(path, L"\\dinput8.dll");
    return LoadLibrary(path);
}

HMODULE chainLoad(std::wstring dllName) {
    if (!dllName.empty()) {
        wchar_t chainPath[MAX_PATH];
        GetModuleFileNameW(NULL, chainPath, MAX_PATH);
        wcscpy_s(chainPath, std::wstring(chainPath).substr(0, std::wstring(chainPath).find_last_of(L"\\/")).c_str());
        lstrcatW(chainPath, L"\\");
        lstrcatW(chainPath, dllName.c_str());
        return LoadLibrary(chainPath);
    }
    return NULL;
}
//https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
inline bool ends_with(std::wstring const& value, std::wstring const& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void load_dll_in_directory(const std::wstring& name, std::vector<HMODULE>& v)
{
    HMODULE temp = nullptr;

    wchar_t FullPath[MAX_PATH];
    wchar_t FullPathFolder[MAX_PATH];
    wchar_t TempPath[MAX_PATH];
    GetModuleFileNameW(NULL, FullPathFolder, MAX_PATH);
    wcscpy_s(FullPathFolder, std::wstring(FullPathFolder).substr(0, std::wstring(FullPathFolder).find_last_of(L"\\/")).c_str());
    lstrcatW(FullPathFolder, L"\\");
    lstrcatW(FullPathFolder, name.c_str());
    lstrcatW(FullPathFolder, L"\\");
    wcscpy_s(FullPath, FullPathFolder);
    lstrcatW(FullPath, L"*");

    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile((LPCWSTR)FullPath, &data)) != INVALID_HANDLE_VALUE) {
        do {
            wcscpy_s(TempPath, FullPathFolder);
            lstrcatW(TempPath, data.cFileName);
            if (ends_with(TempPath, L".dll")) {
                try {
#ifndef NDEBUG 
                    printf("%ls\n", TempPath);
#endif
                    temp = LoadLibrary(TempPath);
                    v.push_back(temp);
                }
                catch (int ex) {
#ifndef NDEBUG 
                    printf("[ERROR][%d]\n", ex);
#endif
                    ex;
                }
            }
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}