#include <ZxHook/Inject.h>
#include <ZxHook/Error.h>
#include <ranges>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>


namespace ZQF::ZxLoader
{
    auto ZxCreateProcess(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, void* pfCreateProcessW, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames) -> BOOL
    {
        size_t dll_count{};
        std::array<const char*, ZXLOADER_MAX_DLL_COUNT> dlls_str_array;
        for (auto&& [seq, dll_str] : std::views::enumerate(aDllNames))
        {
            if (dll_str == nullptr)
            {
                dll_count = seq;
                break;
            }

            dlls_str_array[seq] = dll_str;
        }

        if (dll_count)
        {
            return ::DetourCreateProcessWithDllsW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, static_cast<DWORD>(dll_count), dlls_str_array.data(), (PDETOUR_CREATE_PROCESS_ROUTINEW)pfCreateProcessW);

        }
        else
        {
            return ::CreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
        }
    }

    auto ZxCreateProcess(const wchar_t* wpExePath, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames, wchar_t* wpCmdLine) -> void
    {
        STARTUPINFOW si = { .cb = sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        const auto status = ZxCreateProcess(wpExePath, wpCmdLine, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi, nullptr, aDllNames);
        if (status == FALSE) { ZxHook::SysErrorMsgBox(L"ZxLoader::CreateProcessWithDll(): failed!", true); }
        ::ResumeThread(pi.hThread);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
} // namespace ZQF::ZxLoader
