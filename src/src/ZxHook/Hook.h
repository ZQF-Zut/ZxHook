#pragma once
#include <span>
#include <cstddef>
#include <cstdint>
#include <ZxHook/Mem.h>


namespace ZQF::ZxHook
{
#ifndef _WIN64
    class Transfer
    {
    private:
        VirtualAddress m_vaSrc;
        VirtualAddress m_vaDst;
        std::size_t m_nCoverBytes;

    public:
        Transfer(const VirtualAddress vaSrc, const VirtualAddress vaDst, const std::size_t nCoverBytes);

    public:
        auto CtrlFlow(const std::uint8_t ucAsmCode) const -> void;
        auto AutoReturn() const -> void;
    };

    class Trampoline
    {
    public:
        static auto Detach(void* ppFunc) -> bool;
        static auto Attach(void* ppFunc, void* pDetour, const std::size_t nCoverBytes) -> void;
    };
#endif // _WIN64

    class Detours
    {
    public:
        static auto Begin() -> void;
        static auto Attach(void* ppFunc, void* pDetour) -> void;
        static auto Detach(void* pFunc, void* pDetour) -> void;
        static auto Commit() -> void;
        static auto AfterWith() -> void;
        static auto AttachDirectly(void* ppFunc, void* pDetour) -> void;
        static auto DetachDirectly(void* ppFunc, void* pDetour) -> void;
    };
} // namespace ZQF::ZxHook
