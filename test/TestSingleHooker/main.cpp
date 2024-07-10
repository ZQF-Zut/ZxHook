#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ZxHook/SingleHooker.h>


static auto __stdcall MessageBoxW_Hook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) -> INT
{
    if (::wcscmp(lpText, L"test") == 0)
    {
        lpText = L"Hooked";
    }

    return ZQF::ZxHook::SingleHooker<::MessageBoxW_Hook>::FnRaw(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return ZQF::ZxHook::SingleHooker<::MessageBoxA_Hook>::FnRaw(hWnd, lpText, lpCaption, uType);
}

static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return ZQF::ZxHook::SingleHooker<::ReadScript_Hook>::FnRaw(cpText);
}


auto main() -> int
{
    ZQF::ZxHook::SingleHookerAUX::CommitBeg();
    {
        ZQF::ZxHook::SingleHooker<::MessageBoxA_Hook>::Register(::MessageBoxA);
        ZQF::ZxHook::SingleHooker<::MessageBoxW_Hook>::Register(::MessageBoxW);
        ZQF::ZxHook::SingleHooker<::ReadScript_Hook>::Register(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);
    }
    ZQF::ZxHook::SingleHookerAUX::CommitEnd();

    // ZQF::ZxHook::SingleHooker<::MessageBoxA_Hook>::Commit(::MessageBoxA);
    // ZQF::ZxHook::SingleHooker<::MessageBoxW_Hook>::Commit(::MessageBoxW);
    // ZQF::ZxHook::SingleHooker<::ReadScript_Hook>::Commit(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);

    ::MessageBoxW(nullptr, L"test", nullptr, NULL);
}
