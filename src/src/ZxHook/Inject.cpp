#include <ZxHook/Inject.h>
#include <ZxHook/Error.h>
#include <ranges>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>


namespace ZQF::ZxLoader
{
    auto ZxCreateProcess(const wchar_t* wpExePath, const std::array<const char*, ZXLOADER_MAX_DLL_COUNT>& aDllNames, wchar_t* wpCmdLine) -> void
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

        STARTUPINFOW si = { .cb = sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        if (dll_count)
        {
            if (!::DetourCreateProcessWithDllsW(wpExePath, wpCmdLine, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi, static_cast<DWORD>(dll_count), dlls_str_array.data(), NULL))
            {
                ZxHook::SysErrorMsgBox(L"ZxLoader::CreateProcessWithDll(): failed!", true);
            }
        }
        else
        {
            if (!::CreateProcessW(wpExePath, wpCmdLine, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi))
            {
                ZxHook::SysErrorMsgBox(L"ZxLoader::CreateProcessWithDll(): failed!", true);
            }
        }

        ::ResumeThread(pi.hThread);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
} // namespace ZQF::ZxLoader
