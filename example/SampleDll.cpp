#include <Windows.h>
#include <ZxHook/Hook.h>


using Fn_MessageBoxW_t = decltype(&::MessageBoxW);
static Fn_MessageBoxW_t sg_fnMessageBoxW = ::MessageBoxW;
static INT WINAPI MessageBoxW_Hook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    lpText = L"You've been hooked!";
    return sg_fnMessageBoxW(hWnd, lpText, lpCaption, uType);
}

static void StartHook(HMODULE hDllBase)
{
    ::MessageBoxW(nullptr, L"Sample dll injected!", nullptr, NULL);
    ZQF::ZxHook::Detours::AttachDirectly(&sg_fnMessageBoxW, MessageBoxW_Hook);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ZQF::ZxHook::Detours::AfterWith();
        StartHook(hModule);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" VOID __declspec(dllexport) Dir_A() {}
