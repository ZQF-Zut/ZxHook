#include <ZxHook/Error.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace ZQF::ZxHook
{
    auto SysErrorMsgBox(const char* cpText, bool isExit) -> void
    {
        ::MessageBoxA(nullptr, cpText, nullptr, NULL);
        isExit ? (::ExitProcess(-1)) : ((void)nullptr);
    }

    auto SysErrorMsgBox(const wchar_t* wpText, bool isExit) -> void
    {
        ::MessageBoxW(nullptr, wpText, nullptr, NULL);
        isExit ? (::ExitProcess(-1)) : ((void)nullptr);
    }
}
