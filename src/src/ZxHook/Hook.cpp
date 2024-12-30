#include <ZxHook/Hook.h>
#include <ZxHook/Mem.h>
#include <ZxHook/Error.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>


namespace ZQF::ZxHook
{
#ifndef _WIN64
    Transfer::Transfer(const VirtualAddress vaSrc, const VirtualAddress vaDst, const std::size_t nCoverBytes) :
        m_vaSrc{ vaSrc }, m_vaDst{ vaDst }, m_nCoverBytes{ nCoverBytes }
    {
        const auto status = ZxHook::VirtualProtector::Set(vaSrc, ZxHook::VirtualProperty::ReadWriteExecute, m_nCoverBytes);
        if (status == false) { ZxHook::SysErrorMsgBox(L"ZxHook::Transfer::Ctor: set virtual memory protect failed!", true); return; }
    }

    auto Transfer::CtrlFlow(const std::uint8_t ucAsmCode) const -> void
    {
        m_vaSrc.Put<std::uint8_t>(0, ucAsmCode);
        m_vaSrc.Put<std::size_t>(1, m_vaDst.VA() - m_vaSrc.VA() - 5);
    }

    auto Transfer::AutoReturn() const -> void
    {
        const auto [tpl_va, err] = ZxHook::VirtualAllocator::Alloc(0x1000, ZxHook::VirtualProperty::ReadWriteExecute);
        if (err) { ZxHook::SysErrorMsgBox(L"ZxHook::Transfer::AutoReturn: SysMemAlloc failed!", true); return; }

        tpl_va.Write(0, std::span{ m_vaSrc.Ptr<std::uint8_t*>(), m_nCoverBytes });

        m_vaSrc.Put<std::uint8_t>(0, 0xE9);
        m_vaSrc.Put<std::size_t>(1, tpl_va.VA() - m_vaSrc.VA() - 5);

        tpl_va.Put<std::uint8_t>(m_nCoverBytes + 0, 0xE8);
        tpl_va.Put<std::size_t>(m_nCoverBytes + 1, m_vaDst.VA() - tpl_va.VA() - m_nCoverBytes - 5);

        tpl_va.Put<std::uint8_t>(m_nCoverBytes + 5 + 0, 0xE9);
        tpl_va.Put<std::size_t>(m_nCoverBytes + 5 + 1, (m_vaSrc.VA() + m_nCoverBytes) - (tpl_va.VA() + m_nCoverBytes + 5) - 5);
    }

    auto Trampoline::Detach(void* ppFunc) -> bool
    {
        void** fn_tpl_pp = (void**)ppFunc;
        void* fn_tpl = *fn_tpl_pp;
        return ZxHook::VirtualAllocator::Free(fn_tpl);
    }

    auto Trampoline::Attach(void* ppFunc, void* pDetour, const std::size_t nCoverBytes) -> void
    {
        void** fn_org_pp = (void**)ppFunc;
        const auto fn_org_va = VirtualAddress{ *fn_org_pp };
        const auto [tpl_va, err] = ZxHook::VirtualAllocator::Alloc(nCoverBytes + 5, ZxHook::VirtualProperty::ReadWriteExecute);
        if (err) { return; }
        tpl_va.Write(0, std::span{ fn_org_va.Ptr<std::uint8_t*>(), nCoverBytes });
        ZxHook::Transfer{ tpl_va.VA() + nCoverBytes, fn_org_va.VA() + nCoverBytes, 5}.CtrlFlow(0xE9);
        ZxHook::Transfer{ fn_org_va, pDetour, nCoverBytes }.CtrlFlow(0xE9);
        *fn_org_pp = tpl_va.Ptr<void*>();
    }
#endif // _WIN64

    auto Detours::Begin() -> void
    {
        ::DetourTransactionBegin();
        ::DetourUpdateThread(GetCurrentThread());
    }

    auto Detours::Attach(void* ppFunc, void* pDetour) -> void
    {
        ::DetourAttach((PVOID*)ppFunc, pDetour) != NO_ERROR ? ZxHook::SysErrorMsgBox(L"ZxHook::Detour::Attach: failed!", true) : (void)(0);
    }

    auto Detours::Detach(void* ppFunc, void* pDetour) -> void
    {
        ::DetourDetach((PVOID*)ppFunc, pDetour) != NO_ERROR ? ZxHook::SysErrorMsgBox(L"ZxHook::Detour::Detach: Failed!", true) : (void)(0);
    }

    auto Detours::Commit() -> void
    {
        ::DetourTransactionCommit() != NO_ERROR ? ZxHook::SysErrorMsgBox(L"ZxHook::Detour::Commit: Failed!", true) : (void)(0);
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
