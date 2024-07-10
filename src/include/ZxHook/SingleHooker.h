#pragma once
#include <ZxHook/Hook.h>


namespace ZQF::ZxHook
{
    template<auto FnDetour>
    class SingleHooker
    {
    public:
        using FnType_t = decltype(FnDetour);

    public:
        inline static FnType_t FnRaw;

    public:
        static auto Register(SingleHooker::FnType_t pRawFunc) -> void
        {
            SingleHooker::FnRaw = pRawFunc;
            ZxHook::Detours::Attach(&FnRaw, FnDetour);
        }

        static auto Register(size_t nImageBase, size_t nRva) -> void
        {
            SingleHooker::Register(reinterpret_cast<SingleHooker::FnType_t>(nImageBase + nRva));
        }

        static auto Commit(SingleHooker::FnType_t pRawFunc) -> void
        {
            ZxHook::Detours::Begin();
            SingleHooker::Register(pRawFunc);
            ZxHook::Detours::Commit();
        }

        static auto Commit(size_t nImageBase, size_t nRva) -> void
        {
            SingleHooker::Commit(reinterpret_cast<SingleHooker::FnType_t>(nImageBase + nRva));
        }
    };

    class SingleHookerAUX
    {
    public:
        static auto CommitBeg() -> void
        {
            ZxHook::Detours::Begin();
        }

        static auto CommitEnd() -> void
        {
            ZxHook::Detours::Commit();
        }
    };

}
