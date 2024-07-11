#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <Windows.h>
#include <ZxHook/MultiHooker.h>


// user func
static auto __fastcall FoolThisCall(int /* ecx */, int /* edx */, const char* cpText) -> int
{
    ::printf(cpText);
    return 10;
}

static auto __cdecl FooCdeclCall(const char* cpText) -> int
{
    ::printf(cpText);
    return 10;
}

// hook func type decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
static auto __cdecl ReadScript_Hook(const char* cpText) -> int;
static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int;
static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int;


// MultiHooker type decl
using MyMultiHooker_t = ZQF::ZxHook::MakeMultiHookerType
<
    (void*)MessageBoxA_Hook,
    (void*)CopyFileA_Hook,
    (void*)ReadScript_Hook,
    (void*)FooCdeclCall_Hook,
    (void*)FooThisCall_Hook
>;

using MyMultiHookerIns = ZQF::ZxHook::MakeMultiHookerInstance<MyMultiHooker_t>;

// hook function imp
static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return MyMultiHookerIns::GetRaw<::ReadScript_Hook>()(cpText);
}

static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int
{
    ::MessageBoxA(nullptr, "FooCdeclCall_Hook", nullptr, MB_OK);
    return MyMultiHookerIns::GetRaw<::FooCdeclCall_Hook>()(cpText);
}

static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int
{
    return MyMultiHookerIns::GetRaw<::FooThisCall_Hook>()(ecx, edx, cpText);
}

static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "MessageBoxA Hooked";
    }

    return MyMultiHookerIns::GetRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    MyMultiHookerIns::GetRaw<::MessageBoxA_Hook>()(nullptr, "CopyFileA", nullptr, 0);
    return MyMultiHookerIns::GetRaw<::CopyFileA_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}


static auto __stdcall MessageBoxW_Hook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileW_Hook(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
using YourMultiHooker_t = ZQF::ZxHook::MakeMultiHookerType
<
    (void*)MessageBoxW_Hook,
    (void*)CopyFileW_Hook
>;
using YourMultiHookerIns = ZQF::ZxHook::MakeMultiHookerInstance<YourMultiHooker_t>;

static auto __stdcall MessageBoxW_Hook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) -> INT
{
    if (::wcscmp(lpText, L"test") == 0)
    {
        lpText = L"MessageBoxW Hooked";
    }

    return YourMultiHookerIns::GetRaw<::MessageBoxW_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileW_Hook(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    YourMultiHookerIns::GetRaw<::MessageBoxW_Hook>()(nullptr, L"CopyFileW", nullptr, 0);
    return YourMultiHookerIns::GetRaw<::CopyFileW_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}

auto main() -> int
{
    MyMultiHookerIns::Create<true>();
    MyMultiHookerIns::Add<::CopyFileA_Hook>(::CopyFileA);
    MyMultiHookerIns::Add<::MessageBoxA_Hook>(::MessageBoxA);
    MyMultiHookerIns::Add<::FooThisCall_Hook>(::FoolThisCall);
    MyMultiHookerIns::Add<::FooCdeclCall_Hook>(::FooCdeclCall);
    // MyMultiHookerIns::Add<::ReadScript_Hook>(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);
    MyMultiHookerIns::Commit();

    YourMultiHookerIns::Create();
    YourMultiHookerIns::Add<::MessageBoxW_Hook>(::MessageBoxW);
    YourMultiHookerIns::Add<::CopyFileW_Hook>(::CopyFileW);
    YourMultiHookerIns::Commit();

    ::FooCdeclCall("hook this!");
    ::MessageBoxA(nullptr, "test", nullptr, NULL);
    ::MessageBoxW(nullptr, L"test", nullptr, NULL);
}
