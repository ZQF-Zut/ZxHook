#pragma once
#include <array>
#include <cstdint>


namespace ZQF::ZxLoader
{
    constexpr std::size_t ZXLOADER_MAX_DLL_COUNT = 16;
    auto ZxCreateProcess(const wchar_t* wpExePath, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames = {}, wchar_t* wpCmdLine = {}) -> void;
} // namespace ZQF::ZxLoader
