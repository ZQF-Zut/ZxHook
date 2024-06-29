#pragma once
#include <cstdint>


namespace ZQF::ZxHook
{
#ifndef _WIN64
    class Transfer
    {
    public:
        static auto CtrlFlow(void* pFunc, void* pDest, size_t nCoverSize, uint8_t ucAsmCode) -> void;
        static auto AutoReturn(void* pFunc, void* pDest, size_t nCoverSize) -> void;
    };

    class Trampoline
    {
    private:
        static auto Free(void* ppFunc) -> bool;
        static auto Alloc(void* pFunc, size_t nCopySize) -> void*;

    public:
        static auto Attach(void* ppFunc, size_t nCoverSize, void* pDetour) -> void;
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
