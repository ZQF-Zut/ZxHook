#include <ZxHook/Mem.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <cstring>


namespace ZQF::ZxHook
{
    auto SysMemRead(const std::size_t nVA, void* pBuffer, const std::size_t nBytes) -> bool
    {
        if (ZxHook::SysMemAccess(nVA, nBytes, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memcpy(pBuffer, (const void*)(nVA), nBytes);
            return true;
        }
        return false;
    }

    auto SysMemWrite(const std::size_t nVA, const void* const pData, const std::size_t nBytes) -> bool
    {
        if (ZxHook::SysMemAccess(nVA, nBytes, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memcpy((void*)(nVA), pData, nBytes);
            return true;
        }
        return false;
    }

    auto SysMemFill(const std::size_t nVA, std::uint8_t ucValue, std::size_t nSize) -> bool
    {
        if (ZxHook::SysMemAccess(nVA, nSize, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memset((void*)(nVA), ucValue, nSize);
            return true;
        }
        return false;
    }

    auto SysMemAccess(const std::size_t nVA, std::size_t nSize, std::uint32_t uiAccess, std::uint32_t* pOldAccess) -> bool
    {
        DWORD old{};
        if (::VirtualProtect(LPVOID(nVA), nSize, uiAccess, &old) == TRUE)
        {
            if (pOldAccess != nullptr) { *pOldAccess = old; }
            return true;
        }
        return false;
    }

    auto SysMemAlloc(std::size_t nSize, std::uint32_t uiAccess) -> void*
    {
        return ZxHook::SysMemAlloc(0, nSize, MEM_COMMIT, uiAccess);
    }

    auto SysMemAlloc(const std::size_t nVA, std::size_t nSize, std::uint32_t uiType, std::uint32_t uiAccess) -> void*
    {
        return ::VirtualAlloc(LPVOID(nVA), nSize, uiType, uiAccess);
    }

    auto SysMemFree(const std::size_t nVA) -> bool
    {
        return ::VirtualFree(LPVOID(nVA), 0, MEM_RELEASE) != FALSE;
    }

} // namespace Rut::RxHook
