#pragma once
#include <array>
#include <cstdint>
#include <Windows.h>


namespace ZQF::ZxLoader
{
    constexpr std::size_t ZXLOADER_MAX_DLL_COUNT = 16;
    auto ZxCreateProcess(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames = {}) -> void;
    auto ZxCreateProcess(const wchar_t* wpExePath, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames = {}, wchar_t* wpCmdLine = {}) -> void;
} // namespace ZQF::ZxLoader
