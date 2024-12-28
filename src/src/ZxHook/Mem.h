#pragma once
#include <cstdint>
#include <cstring>
#include <span>


namespace ZQF::ZxHook
{
    auto SysMemRead(const std::size_t nVA, void* pBuffer, const std::size_t nBytes) -> bool;
    auto SysMemWrite(const std::size_t nVA, const void* const pData, const std::size_t nBytes) -> bool;
    auto SysMemFill(const std::size_t nVA, std::uint8_t ucValue, std::size_t nSize) -> bool;
    auto SysMemAccess(const std::size_t nVA, std::size_t nSize, std::uint32_t uiAccess, std::uint32_t* pOldAccess = nullptr) -> bool;
    auto SysMemAlloc(std::size_t nSize, std::uint32_t uiAccess) -> void*;
    auto SysMemAlloc(const std::size_t nVA, std::size_t nSize, std::uint32_t uiType, std::uint32_t uiAccess) -> void*;
    auto SysMemFree(const std::size_t nVA) -> bool;

    template<class T, std::size_t S>
    auto SysMemRead(const std::size_t nVA, const std::span<T, S> spBuffer) -> bool
    {
        return ZxHook::SysMemRead(nVA, spBuffer.data(), spBuffer.size_bytes());
    }

    template<class T, std::size_t S>
    auto SysMemWrite(const std::size_t nVA, const std::span<T, S> spData) -> bool
    {
        return ZxHook::SysMemWrite(nVA, spData.data(), spData.size_bytes());
    }

    template <bool isBackward>
    auto MemSearch(const std::span<std::uint8_t> spSource, const std::span<std::uint8_t> spMatch) -> const void*
    {
        if (spSource.empty() && spMatch.empty()) { return nullptr; }

        if constexpr (isBackward)
        {
            for (std::size_t ite = 0; ite < spSource.size(); ite++)
            {
                if (::memcmp(spMatch.data(), spSource.data() - ite, spMatch.size()) == 0)
                {
                    return (spSource.data() - ite + 1);
                }
            }
        }
        else
        {
            for (std::size_t ite = 0; ite < spSource.size(); ite++)
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
