#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <array>
#include <detours.h>
#include <stdexcept>


namespace ZQF::ZxHook
{
    template <auto vcHookFuncList>
    class Hvec
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
        std::array<Entry, vcHookFuncList.size()> m_aIndex;

        auto GetEntry(size_t nIndex) noexcept -> Entry&
        {
            return m_aIndex[nIndex];
        }

        template <auto pHokFuncPtr>
        static constexpr auto GetIndex() -> size_t
        {
            size_t seq = 0;
            for (auto hook_func_ptr : vcHookFuncList)
            {
                if ((decltype(hook_func_ptr))pHokFuncPtr == hook_func_ptr)
                {
                    return seq;
                }
                seq++;
            }

            throw std::runtime_error("Hvec::GetIndex: not find hook function ptr");
        }

    public:
        template <auto pHookFuncPtr>
        auto Push(decltype(pHookFuncPtr) pRawFuncPtr) noexcept -> auto&
        {
            constexpr size_t index = Hvec::GetIndex<pHookFuncPtr>();
            m_aIndex[index].pRaw = reinterpret_cast<void*>(pRawFuncPtr);
            m_aIndex[index].pHook = reinterpret_cast<void*>(pHookFuncPtr);
            return this->GetEntry(index);
        }

        template <auto pHokFuncPtr>
        auto GetRaw() noexcept
        {
            constexpr size_t index = Hvec::GetIndex<pHokFuncPtr>();
            return reinterpret_cast<decltype(pHokFuncPtr)>(this->GetEntry(index).pRaw);
        }

        template <Hvec::Backend eBackend = Hvec::Backend::Detours>
        auto AttachAll() -> void
        {
            if constexpr (eBackend == Hvec::Backend::Detours)
            {
                ::DetourTransactionBegin();
                ::DetourUpdateThread(GetCurrentThread());

                for (auto& entry : m_aIndex)
                {
                    ::DetourAttach(&entry.pRaw, entry.pHook);
                }

                ::DetourTransactionCommit();
            }
            else if constexpr (eBackend == Hvec::Backend::MinHook)
            {
                static_assert(false, "Hvec::Attach<Hvec::Backend::MinHook>: not implementation");
            }
            else
            {
                static_assert(false, "Hvec::Attach<>: unknown backend");
            }
        }
    };

} // namespace ZxHook


