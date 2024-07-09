#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <Windows.h>
#include <ZxHook/MultiHooker.h>


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

// custom multi hooker type decl
static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)->INT;
static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)->BOOL;
static auto __cdecl ReadScript_Hook(const char* cpText) -> int;
static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int;
static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int;
using MyMakeMultiHooker_t = ZQF::ZxHook::MakeMultiHookerType
<
    (void*)MessageBoxA_Hook,
    (void*)CopyFileA_Hook,
    (void*)ReadScript_Hook,
    (void*)FooCdeclCall_Hook,
    (void*)FooThisCall_Hook
>;

// hook func type
using Fn_ReadScript_t = decltype(&::ReadScript_Hook);

// instantiate custom multi hooker type 
static auto sg_MultiHooker = MyMakeMultiHooker_t{};

// hvec auxiliary func
template <auto pHokFuncPtr>
static auto GetHvecRaw() noexcept
{
    return sg_MultiHooker.GetRaw<pHokFuncPtr>();
}

// hook function imp
static auto __cdecl ReadScript_Hook(const char* cpText) -> int
{
    return GetHvecRaw<::ReadScript_Hook>()(cpText);
}

static auto __cdecl FooCdeclCall_Hook(const char* cpText) -> int
{
    ::MessageBoxA(nullptr, "FooCdeclCall_Hook", nullptr, MB_OK);
    return GetHvecRaw<::FooCdeclCall_Hook>()(cpText);
}

static auto __fastcall FooThisCall_Hook(int ecx, int edx, const char* cpText) -> int
{
    return GetHvecRaw<::FooThisCall_Hook>()(ecx, edx, cpText);
}

static auto __stdcall MessageBoxA_Hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) -> INT
{
    if (::strcmp(lpText, "test") == 0)
    {
        lpText = "Hooked";
    }

    return GetHvecRaw<::MessageBoxA_Hook>()(hWnd, lpText, lpCaption, uType);
}

static auto __stdcall CopyFileA_Hook(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) -> BOOL
{
    GetHvecRaw<::MessageBoxA_Hook>()(nullptr, "CopyFileA", nullptr, 0);
    return GetHvecRaw<::CopyFileA_Hook>()(lpExistingFileName, lpNewFileName, bFailIfExists);
}


auto main() -> int
{
    ::sg_MultiHooker.Add<::CopyFileA_Hook>(::CopyFileA);
    ::sg_MultiHooker.Add<::MessageBoxA_Hook>(::MessageBoxA);
    ::sg_MultiHooker.Add<::FooThisCall_Hook>(::FoolThisCall);
    ::sg_MultiHooker.Add<::FooCdeclCall_Hook>(::FooCdeclCall);
    ::sg_MultiHooker.Add<::ReadScript_Hook>(reinterpret_cast<Fn_ReadScript_t>(::GetModuleHandleA(nullptr) + 0x100));
    ::sg_MultiHooker.Commit();

    ::FooCdeclCall("hook this!");
    ::MessageBoxA(nullptr, "test", nullptr, NULL);
    //::CopyFileA("D:\\my_file.txt", "D:\\my_file_new.txt", static_cast<BOOL>(false));
}
