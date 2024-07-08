#include <ZxHook/Mem.h>
#include <ZxHook/Error.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <cstring>


namespace ZQF::ZxHook
{
    auto SysMemRead(void* pAddress, void* pBuffer, size_t nSize) -> bool
    {
        if (SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memcpy(pBuffer, pAddress, nSize);
            return true;
        }
        return false;
    }

    auto SysMemWrite(void* pAddress, void* pBuffer, size_t nSize) -> bool
    {
        if (SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memcpy(pAddress, pBuffer, nSize);
            return true;
        }
        return false;
    }

    auto SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize) -> bool
    {
        if (SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr))
        {
            std::memset(pAddress, ucValue, nSize);
            return true;
        }
        return false;
    }

    auto SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess) -> bool
    {
        DWORD old{};
        if (::VirtualProtect(pAddress, nSize, uiAccess, &old) == TRUE)
        {
            if (pOldAccess != nullptr) { *pOldAccess = old; }
            return true;
        }
        return false;
    }

    auto SysMemAlloc(size_t nSize, uint32_t uiAccess) -> void*
    {
        return SysMemAlloc(nullptr, nSize, MEM_COMMIT, uiAccess);
    }

    auto SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess) -> void*
    {
        return ::VirtualAlloc(pAddress, nSize, uiType, uiAccess);
    }

    auto SysMemFree(void* pAddress) -> bool
    {
        return ::VirtualFree(pAddress, 0, MEM_RELEASE) != FALSE;
    }

    auto SysMemRead(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void
    {
        SysMemRead(pAddress, pBuffer, nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
    }

    auto SysMemWrite(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void
    {
        SysMemWrite(pAddress, pBuffer, nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
    }

    auto SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize, const wchar_t* wpErrorMsg, bool isExit) -> void
    {
        SysMemFill(pAddress, ucValue, nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
    }

    auto SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess, const wchar_t* wpErrorMsg, bool isExit) -> void
    {
        SysMemAccess(pAddress, nSize, uiAccess, pOldAccess) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
    }

    auto SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess, const wchar_t* wpErrorMsg, bool isExit) -> void*
    {
        void* buffer_ptr = SysMemAlloc(pAddress, nSize, uiType, uiAccess);
        if (buffer_ptr == nullptr) { SysErrorMsgBox(wpErrorMsg, isExit); }
        return buffer_ptr;
    }

} // namespace Rut::RxHook
