# Zut::ZxHook

## Quick Start
- single hook
```c++
#include <ZxHook/MultiHooker.h>


// hook func decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT;

// create hooker
static auto sg_MsgBoxAHook = ZQF::ZxHook::MultiHooker<(void*)MessageBoxA_Hook>{};

// hook func imp
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return sg_MsgBoxAHook.GetRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

auto main(void) -> int
{
    sg_MsgBoxAHook.Reg<::MessageBoxA_Hook>(::MessageBoxA);
    sg_MsgBoxAHook.Commit();
}
```

- multi hook
```c++
#include <ZxHook/MultiHooker.h>


// hook func decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
static auto __cdecl ReadScript_Hook(const char* cpText) -> int;
using MyHookerType = ZQF::ZxHook::MakeMultiHookerType
<
    (void*)MessageBoxA_Hook,
    (void*)CopyFileA_Hook,
    (void*)ReadScript_Hook
>;

// hook func type
using Fn_ReadScript_t = decltype(&::ReadScript_Hook);

// create hooker
static auto sg_MyHooker = MyHookerType{};

// hook fun imp
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return sg_MyHooker.GetRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    sg_MyHooker.GetRaw<::MessageBoxA_Hook>()(nullptr, "CopyFileA Hooked!", nullptr, MB_OK);
    return sg_MyHooker.GetRaw<::CopyFileA_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}

static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return sg_MyHooker.GetRaw<::ReadScript_Hook>()(cpText);
}

auto main(void) -> int
{
    sg_MyHooker.Reg<::MessageBoxA_Hook>(::MessageBoxA);
    sg_MyHooker.Reg<::CopyFileA_Hook>(::CopyFileA);
    sg_MyHooker.Reg<::ReadScript_Hook>(reinterpret_cast<Fn_ReadScript_t>(::GetModuleHandleW(nullptr) + 0x532C));
    sg_MyHooker.Commit();
}
```
