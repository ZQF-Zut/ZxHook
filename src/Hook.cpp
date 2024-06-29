#include <ZxHook/Hook.h>
#include <ZxHook/Mem.h>
#include <ZxHook/Error.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>


namespace ZQF::ZxHook
{
#ifndef _WIN64
    auto Transfer::CtrlFlow(void* pFunc, void* pDest, size_t nCoverSize, uint8_t ucAsmCode) -> void
    {
        SysMemAccess(pFunc, nCoverSize, PAGE_EXECUTE_READWRITE, nullptr, L"Transfer::Set Failed!", true);
        *(uint8_t*)((uint8_t*)pFunc + 0) = ucAsmCode;
        *(uint32_t*)((uint8_t*)pFunc + 1) = (uint32_t)pDest - (uint32_t)pFunc - 5;
        (nCoverSize > 0x5) ? (void)memset((uint8_t*)pFunc + 0x5, 0x90, nCoverSize - 0x5) : (void)nullptr;
    }

    auto Transfer::AutoReturn(void* pFunc, void* pDest, size_t nCoverSize) -> void
    {
        size_t rva = 0;
        uint8_t raw_jmp_asm[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        uint8_t ret_jmp_asm[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        uint8_t tar_cal_asm[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };

        SysMemAccess(pFunc, 0x1000, PAGE_EXECUTE_READWRITE, nullptr, L"Transfer::Set SysMemAccess Failed!", true);

        uint8_t* code_buffer = (uint8_t*)SysMemAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE, L"Transfer::Set SysMemAlloc Failed!", true);

        // Copy the Code for the original address to alloc address
        memcpy(code_buffer, pFunc, nCoverSize);

        // Write Jmp Code
        rva = (size_t)code_buffer - (size_t)pFunc - sizeof(raw_jmp_asm);
        memcpy(raw_jmp_asm + 1, &rva, sizeof(size_t));
        memcpy(pFunc, raw_jmp_asm, sizeof(raw_jmp_asm));

        // Write Call TarFunc Code
        rva = (size_t)pDest - (size_t)(code_buffer + nCoverSize) - sizeof(tar_cal_asm);
        memcpy(tar_cal_asm + 1, &rva, sizeof(size_t));
        memcpy(code_buffer + nCoverSize, tar_cal_asm, sizeof(tar_cal_asm));

        // Write Ret Code
        rva = ((size_t)pFunc + nCoverSize) - (size_t)(code_buffer + nCoverSize + sizeof(tar_cal_asm)) - sizeof(ret_jmp_asm);
        memcpy(ret_jmp_asm + 1, &rva, sizeof(size_t));
        memcpy(code_buffer + nCoverSize + sizeof(tar_cal_asm), ret_jmp_asm, sizeof(ret_jmp_asm));
    }



    auto Trampoline::Free(void* ppFunc) -> bool
    {
        void** fn_tpl_pp = (void**)ppFunc;
        void* fn_tpl = *fn_tpl_pp;
        return SysMemFree(fn_tpl);
    }

    auto Trampoline::Alloc(void* pFunc, size_t nCopySize) -> void*
    {
        uint32_t copy_src_func_asm_size = (uint32_t)nCopySize;
        uint32_t trampoline_func_size = copy_src_func_asm_size + 5;
        void* tpl_func_buffer = SysMemAlloc(trampoline_func_size, PAGE_EXECUTE_READWRITE);
        if (tpl_func_buffer)
        {
            if (memcpy(tpl_func_buffer, pFunc, copy_src_func_asm_size))
            {
                Transfer::CtrlFlow((uint8_t*)tpl_func_buffer + copy_src_func_asm_size, (uint8_t*)pFunc + copy_src_func_asm_size, 5, 0xE9);
                return tpl_func_buffer;
            }
        }
        return nullptr;
    }

    auto Trampoline::Attach(void* ppFunc, size_t nCoverSize, void* pDetour) -> void
    {
        void** fn_org_pp = (void**)ppFunc;
        void* fn_org = *fn_org_pp;
        *fn_org_pp = Trampoline::Alloc(fn_org, nCoverSize);
        Transfer::CtrlFlow(fn_org, pDetour, nCoverSize, 0xE9);
    }
#endif // _WIN64

    auto Detours::Begin() -> void
    {
        ::DetourTransactionBegin();
        ::DetourUpdateThread(GetCurrentThread());
    }

    auto Detours::Attach(void* ppFunc, void* pDetour) -> void
    {
        ::DetourAttach((PVOID*)ppFunc, pDetour) != NO_ERROR ? SysErrorMsgBox(L"DetourAttach Failed!!", true) : (void)(0);
    }

    auto Detours::Detach(void* ppFunc, void* pDetour) -> void
    {
        ::DetourDetach((PVOID*)ppFunc, pDetour) != NO_ERROR ? SysErrorMsgBox(L"DetourDetach Failed!!", true) : (void)(0);
    }

    auto Detours::Commit() -> void
    {
        ::DetourTransactionCommit() != NO_ERROR ? SysErrorMsgBox(L"DetourTransactionCommit Failed!!", true) : (void)(0);
    }

    auto Detours::AfterWith() -> void
    {
        ::DetourRestoreAfterWith();
    }

    auto Detours::AttachDirectly(void* ppFunc, void* pDetour) -> void
    {
        Detours::Begin();
        Detours::Attach(ppFunc, pDetour);
        Detours::Commit();
    }

    auto Detours::DetachDirectly(void* ppFunc, void* pDetour) -> void
    {
        Detours::Begin();
        Detours::Detach(ppFunc, pDetour);
        Detours::Commit();
    }

} // namespace ZQF::ZxHook
