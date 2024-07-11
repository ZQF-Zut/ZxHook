# Zut::ZxHook

## Quick Start
- single hook
```c++
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ZxHook/SingleHooker.h>


static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return ZQF::ZxHook::SingleHooker<::MessageBoxA_Hook>::FnRaw(hWnd, lpText, lpCaption, uType);
}


auto main(void) -> int
{
    ZQF::ZxHook::SingleHooker<::MessageBoxA_Hook>::Commit(::MessageBoxA);
}
```

- multi hook
```c++
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ZxHook/MultiHooker.h>


// hook func decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
static auto __cdecl ReadScript_Hook(const char* cpText) -> int;
using MyMultiHooker_t = ZQF::ZxHook::MakeMultiHookerType
<
    (void*)MessageBoxA_Hook,
    (void*)CopyFileA_Hook,
    (void*)ReadScript_Hook
>;

using MyMultiHookerIns = ZQF::ZxHook::MultiHookerInstance<MyMultiHooker_t>;

// hook fun imp
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return MyMultiHookerIns::GetRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    MyMultiHookerIns::GetRaw<::MessageBoxA_Hook>()(nullptr, "CopyFileA Hooked!", nullptr, MB_OK);
    return MyMultiHookerIns::GetRaw<::CopyFileA_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}

static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return MyMultiHookerIns::GetRaw<::ReadScript_Hook>()(cpText);
}


auto main(void) -> int
{
    MyMultiHookerIns::Create();
    MyMultiHookerIns::Add<::CopyFileA_Hook>(::CopyFileA);
    MyMultiHookerIns::Add<::MessageBoxA_Hook>(::MessageBoxA);
    // MyMultiHookerIns::Add<::ReadScript_Hook>(reinterpret_cast<size_t>(::GetModuleHandleA(nullptr)), 0x532C);
    MyMultiHookerIns::Commit();
}
```
