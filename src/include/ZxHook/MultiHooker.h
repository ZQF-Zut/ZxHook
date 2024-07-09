#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <array>
#include <detours.h>
#include <stdexcept>


namespace ZQF::ZxHook
{
    template <void*... HookFuns>
    class MultiHooker
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
        std::array<Entry, sizeof...(HookFuns)> m_aIndex;

        auto GetEntry(size_t nIndex) noexcept -> Entry&
        {
            return m_aIndex[nIndex];
        }

        template <auto pHookFuncPtr>
        static constexpr auto GetIndex() -> size_t
        {
            constexpr void* hook_func_array[] = { HookFuns... };

            size_t seq = 0;
            for (auto hook_func_ptr : hook_func_array)
            {
                if ((void*)pHookFuncPtr == hook_func_ptr)
                {
                    return seq;
                }
                seq++;
            }

            throw std::runtime_error("MultiHooker::GetIndex: not find hook function ptr");
        }

    public:
        template <auto pHookFuncPtr>
        auto Reg(decltype(pHookFuncPtr) pRawFuncPtr) noexcept -> auto&
        {
            constexpr size_t index = MultiHooker::GetIndex<pHookFuncPtr>();
            m_aIndex[index].pRaw = reinterpret_cast<void*>(pRawFuncPtr);
            m_aIndex[index].pHook = reinterpret_cast<void*>(pHookFuncPtr);
            return this->GetEntry(index);
        }

        template <auto pHokFuncPtr>
        auto GetRaw() noexcept
        {
            constexpr size_t index = MultiHooker::GetIndex<pHokFuncPtr>();
            return reinterpret_cast<decltype(pHokFuncPtr)>(this->GetEntry(index).pRaw);
        }

        template <MultiHooker::Backend eBackend = MultiHooker::Backend::Detours>
        auto Commit() -> void
        {
            if constexpr (eBackend == MultiHooker::Backend::Detours)
            {
                ::DetourTransactionBegin();
                ::DetourUpdateThread(GetCurrentThread());

                for (auto& entry : m_aIndex)
                {
                    ::DetourAttach(&entry.pRaw, entry.pHook);
                }

                ::DetourTransactionCommit();
            }
            else if constexpr (eBackend == MultiHooker::Backend::MinHook)
            {
                static_assert(false, "MultiHooker::Attach<MultiHooker::Backend::MinHook>: not implementation");
            }
            else
            {
                static_assert(false, "MultiHooker::Attach<>: unknown backend");
            }
        }
    };

    template <void*... HookFuns>
    using MakeMultiHookerType = MultiHooker<HookFuns...>;

} // namespace ZxHook



