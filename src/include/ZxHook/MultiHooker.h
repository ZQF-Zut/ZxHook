#pragma once
#include <memory>
#include <ZxHook/Hook.h>


namespace ZQF::ZxHook::Private
{
    template <auto Func, auto FuncList>
    struct FuncIndex
    {
        static consteval auto GetIndex() -> size_t
        {
            for (size_t i = 0; i < FuncList.size(); i++)
            {
                if ((void*)Func == FuncList[i]) { return i; }
            }
        }

        static constexpr auto Val = FuncIndex::GetIndex();
    };

    template<auto Func, auto FuncList>
    constexpr auto FuncIndex_V = FuncIndex<Func, FuncList>::Val;



    template <void*... HookFuns>
    class MultiHookerImp
    {
    public:
        enum class Backend
        {
            Detours,
            MinHook
        };
    private:
        struct Entry
        {
            void* pRaw;
            void* pHook;
        };
        Entry m_aIndex[sizeof...(HookFuns)]{};

        auto GetEntry(size_t nIndex) noexcept -> Entry&
        {
            return m_aIndex[nIndex];
        }

        template <auto pHookFuncPtr>
        static consteval auto GetIndex() -> size_t
        {
            constexpr void* hook_func_array[] = { HookFuns... };

            for (size_t i = 0; i < sizeof...(HookFuns); i++)
            {
                if ((void*)pHookFuncPtr == hook_func_array[i]) { return i; }
            }
        }

    public:
        template <auto pHookFuncPtr>
        auto Add(decltype(pHookFuncPtr) pRawFuncPtr) noexcept -> auto&
        {
            constexpr auto index = MultiHookerImp::GetIndex<pHookFuncPtr>();
            m_aIndex[index].pRaw = reinterpret_cast<void*>(pRawFuncPtr);
            m_aIndex[index].pHook = reinterpret_cast<void*>(pHookFuncPtr);
            return this->GetEntry(index);
        }

        template <auto pHookFuncPtr>
        auto Add(size_t nImageBase, size_t nRva) noexcept -> auto&
        {
            return this->Add<pHookFuncPtr>(reinterpret_cast<decltype(pHookFuncPtr)>(nImageBase + nRva));
        }

        template <auto pHokFuncPtr>
        auto GetRaw() noexcept
        {
            constexpr auto index = MultiHookerImp::GetIndex<pHokFuncPtr>();
            return reinterpret_cast<decltype(pHokFuncPtr)>(this->GetEntry(index).pRaw);
        }

        template <MultiHookerImp::Backend eBackend = MultiHookerImp::Backend::Detours>
        auto Commit() -> void
        {
            if constexpr (eBackend == MultiHookerImp::Backend::Detours)
            {
                ZxHook::Detours::Begin();

                for (auto& entry : m_aIndex)
                {
                    if (entry.pRaw == nullptr) { continue; }
                    ZxHook::Detours::Attach(&entry.pRaw, entry.pHook);
                }

                ZxHook::Detours::Commit();
            }
            else if constexpr (eBackend == MultiHookerImp::Backend::MinHook)
            {
                static_assert(false, "MultiHookerImp::Attach<MultiHookerImp::Backend::MinHook>: not implementation");
            }
            else
            {
                static_assert(false, "MultiHookerImp::Attach<>: unknown backend");
            }
        }
    };

} // namespace ZQF::ZxHook::Private


namespace ZQF::ZxHook
{

    template <void*... HookFuns>
    using MakeMultiHookerType = Private::MultiHookerImp<HookFuns...>;


    template<class MutiHookerType>
    class MultiHookerInstance
    {
    private:
        inline static MutiHookerType* sg_pMultiHooker;

    public:
        using Backend = MutiHookerType::Backend;

    public:
        MultiHookerInstance() {}

        template<bool isCompact = false>
        static auto Create() -> void
        {
            if constexpr (isCompact)
            {
                sg_pMultiHooker = new MutiHookerType{};
            }
            else
            {
                static auto inst = MutiHookerType{};
                sg_pMultiHooker = &inst;
            }
        }

        template <MultiHookerInstance::Backend eBackend = MultiHookerInstance::Backend::Detours>
        static auto Commit() -> void
        {
            MultiHookerInstance::Get().Commit<eBackend>();
        }


        static auto Get() -> auto&
        {
            return *sg_pMultiHooker;
        }

        template <auto pHokFuncPtr>
        static auto GetRaw()
        {
            return MultiHookerInstance::Get().GetRaw<pHokFuncPtr>();
        }

        template <auto pHookFuncPtr>
        static auto Add(decltype(pHookFuncPtr) pRawFuncPtr) noexcept -> void
        {
            MultiHookerInstance::Get().Add<pHookFuncPtr>(pRawFuncPtr);
        }

        template <auto pHookFuncPtr>
        static auto Add(size_t nImageBase, size_t nRva) noexcept -> void
        {
            MultiHookerInstance::Get().Add<pHookFuncPtr>(nImageBase, nRva);
        }
    };

} // namespace ZxHook



