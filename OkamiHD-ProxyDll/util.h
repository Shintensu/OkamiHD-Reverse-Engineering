#pragma once
#include "pch.h"

#include <Windows.h>
#include <string>

//macro from amir who got it from someone
#define call(f,...) (((intptr_t(*)(uintptr_t,...))(f))((uintptr_t)__VA_ARGS__))

std::wstring ExePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

void* buildPtrTo(std::uint64_t n_args, ...)
{
    va_list vl;
    va_start(vl, n_args);
    std::uint64_t bld = *(std::uint64_t*)va_arg(vl, std::uint64_t);
    for (int i = 1; i < n_args - 1; i++)
    {
        bld = *(std::uint64_t*)(bld + va_arg(vl, std::uint64_t));
    }
    bld = (bld + va_arg(vl, std::uint64_t));
    va_end(vl);
    return (void*)bld;
}

template<class T>
T buildPtrToValue(std::uint64_t n_args, ...)
{
    va_list vl;
    va_start(vl, n_args);
    std::uint64_t bld = *(std::uint64_t*)va_arg(vl, std::uint64_t);
    for (int i = 1; i < n_args; i++)
    {
        bld = *(std::uint64_t*)(bld + va_arg(vl, std::uint64_t));
    }
    va_end(vl);
    return (T)bld;
}
