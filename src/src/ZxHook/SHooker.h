#pragma once
#include <cstddef>


// SingleHooker
namespace ZQF::ZxHook
{
    class SHookerDetour
    {
    public:
        static auto CommitBeg() -> void;
        static auto CommitEnd() -> void;
        static auto Attach(void* ppFunc, void* pDetour) -> void;
        static auto Detach(void* ppFunc, void* pDetour) -> void;
        static auto AfterWith() -> void;
    };

    template<auto FnDetour>
    class SHooker
    {
    public:
        using FnType_t = decltype(FnDetour);

    public:
        inline static FnType_t FnRaw;

    public:
        static auto Attach(SHooker::FnType_t pRawFunc) -> void
        {
            SHooker::FnRaw = pRawFunc;
            SHookerDetour::Attach(&FnRaw, FnDetour);
        }

        static auto Attach(const std::size_t nRawFuncVA) -> void
        {
            SHooker::Attach(reinterpret_cast<SHooker::FnType_t>(nRawFuncVA));
        }

        static auto Attach(const std::size_t nImageBase, const std::size_t nRva) -> void
        {
            SHooker::Attach(reinterpret_cast<SHooker::FnType_t>(nImageBase + nRva));
        }

        auto Detach() -> void
        {
            SHookerDetour::Detach(&FnRaw, FnDetour);
        }

        static auto Commit(SHooker::FnType_t pRawFunc) -> void
        {
            SHookerDetour::CommitBeg();
            SHooker::Attach(pRawFunc);
            SHookerDetour::CommitEnd();
        }

        static auto Commit(const std::size_t nRawFuncVA) -> void
        {
            SHooker::Commit(reinterpret_cast<SHooker::FnType_t>(nRawFuncVA));
        }

        static auto Commit(const std::size_t nImageBase, const std::size_t nRva) -> void
        {
            SHooker::Commit(reinterpret_cast<SHooker::FnType_t>(nImageBase + nRva));
        }
    };
}
