#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ZxHook/SHooker.h>

#include <iostream>
#include <print>


static auto __stdcall MessageBoxW_Hook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) -> INT
{
    if (::wcscmp(lpText, L"test") == 0)
    {
        lpText = L"Hooked";
    }

    return ZQF::ZxHook::SHooker<::MessageBoxW_Hook>::FnRaw(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return ZQF::ZxHook::SHooker<::MessageBoxA_Hook>::FnRaw(hWnd, lpText, lpCaption, uType);
}

static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return ZQF::ZxHook::SHooker<::ReadScript_Hook>::FnRaw(cpText);
}

struct MyFool
{
    int a;
    char b;

    MyFool()
    {
        this->a = 100;
        this->b = 50;
    }

    int Tx(int x) {
        std::println("org:{}", this->a + x);
        return this->b + x;
    }
};


struct MyFool_Hook
{
    int a;
    int b;

    static  int __fastcall Tx_Hook(MyFool_Hook* thisx, void* edx, int x) {
        std::println("hooked:{}", thisx->a + x);
        return ZQF::ZxHook::SHooker<MyFool_Hook::Tx_Hook>::FnRaw(thisx, edx, thisx->b + x);
    }
};


auto main() -> int
{
    ZQF::ZxHook::SHookerDetour::CommitBeg();
    {
        // ZQF::ZxHook::SHooker<MyFool_Hook::Tx_Hook>::Attach((decltype(&MyFool_Hook::Tx_Hook))(MyFool::Tx));
        ZQF::ZxHook::SHooker<::MessageBoxA_Hook>::Attach(::MessageBoxA);
        ZQF::ZxHook::SHooker<::MessageBoxW_Hook>::Attach(::MessageBoxW);
        // ZQF::ZxHook::SHooker<::ReadScript_Hook>::Attach(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);
    }
    ZQF::ZxHook::SHookerDetour::CommitEnd();

    MyFool x;
    x.Tx(10);

    // ZQF::ZxHook::SHooker<::MessageBoxA_Hook>::Commit(::MessageBoxA);
    // ZQF::ZxHook::SHooker<::MessageBoxW_Hook>::Commit(::MessageBoxW);
    // ZQF::ZxHook::SHooker<::ReadScript_Hook>::Commit(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);

    ::MessageBoxW(nullptr, L"test", nullptr, NULL);
}
