#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <Windows.h>
#include <ZxHook/Hvec.h>


// user func
static auto __fastcall FoolThisCall(int /* ecx */, int /* edx */, const char* cpText) -> int
{
    std::cout << cpText;
    return 10;
}

static auto __cdecl FooCdeclCall(const char* cpText) -> int
{
    std::cout << cpText;
    return 10;
}

// hook function decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
static auto __cdecl ReadScript_Hook(const char* cpText) -> int;
static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int;
static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int;

// manager defined
static auto GetHvec() -> auto&
{
    constexpr std::array<void*, 5> hook_list
    {
        (void*)MessageBoxA_Hook,
        (void*)CopyFileA_Hook,
        (void*)ReadScript_Hook,
        (void*)FooCdeclCall_Hook,
        (void*)FooThisCall_Hook
    };

    static ZQF::ZxHook::Hvec<hook_list> g_Hvec;
    return g_Hvec;
}

// hook function imp
static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return GetHvec().GetRaw<::ReadScript_Hook>()(cpText);
}

static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int
{
    std::cout << "Hooked\n";
    return GetHvec().GetRaw<::FooCdeclCall_Hook>()(cpText);
}

static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int
{
    return GetHvec().GetRaw<::FooThisCall_Hook>()(ecx, edx, cpText);
}

static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return GetHvec().GetRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    GetHvec().GetRaw<::MessageBoxA_Hook>()(nullptr, "CopyFileA", nullptr, 0);
    return GetHvec().GetRaw<::CopyFileA_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}

auto main() -> int
{
    ::GetHvec().Push<::CopyFileA_Hook>(::CopyFileA);
    ::GetHvec().Push<::MessageBoxA_Hook>(::MessageBoxA);
    ::GetHvec().Push<::FooThisCall_Hook>(::FoolThisCall);
    ::GetHvec().Push<::FooCdeclCall_Hook>(FooCdeclCall);
    ::GetHvec().Push<::ReadScript_Hook>(reinterpret_cast<decltype(&::ReadScript_Hook)>(::GetModuleHandleA(nullptr) + 0x100));
    ::GetHvec().AttachAll();

    ::FooCdeclCall("hook this!");
    ::MessageBoxA(nullptr, "test", nullptr, NULL);
    ::CopyFileA("D:\\my_file.txt", "D:\\my_file_new.txt", static_cast<BOOL>(false));
}
