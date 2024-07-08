#pragma once
#include <cstdint>
#include <cstring>
#include <span>


namespace ZQF::ZxHook
{
    auto SysMemRead(void* pAddress, void* pBuffer, size_t nSize) -> bool;
    auto SysMemWrite(void* pAddress, void* pBuffer, size_t nSize) -> bool;
    auto SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize) -> bool;
    auto SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess) -> bool;
    auto SysMemAlloc(size_t nSize, uint32_t uiAccess) -> void*;
    auto SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess) -> void*;
    auto SysMemFree(void* pAddress) -> bool;

    auto SysMemRead(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void;
    auto SysMemWrite(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void;
    auto SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void;
    auto SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess, const wchar_t* wpErrorMsg, bool isExit) -> void;
    auto SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess, const wchar_t* wpErrorMsg, bool isExit) -> void*;

    template <bool isBackward>
    auto MemSearch(const std::span<uint8_t> spSource, const std::span<uint8_t> spMatch) -> const void*
    {
        if (spSource.empty() && spMatch.empty()) { return nullptr; }

        if constexpr (isBackward)
        {
            for (size_t ite = 0; ite < spSource.size(); ite++)
            {
                if (::memcmp(spMatch.data(), spSource.data() - ite, spMatch.size()) == 0)
                {
                    return (spSource.data() - ite + 1);
                }
            }
        }
        else
        {
            for (size_t ite = 0; ite < spSource.size(); ite++)
            {
                if (::memcmp(spMatch.data(), spSource.data() + ite, spMatch.size()) == 0)
                {
                    return (spSource.data() + ite - 1);
                }
            }
        }

        return nullptr;
    }
} // namespace ZQF::ZxHook
