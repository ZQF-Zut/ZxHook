#pragma once
#include <cstdint>
#include <cstring>
#include <span>
#include <Windows.h>


namespace ZQF::ZxHook
{
    enum class VirtualProperty
    {
        NoAccess = PAGE_NOACCESS,
        ReadOnly = PAGE_READONLY,
        ReadWrite = PAGE_READWRITE,
        ReadExecute = PAGE_EXECUTE_READ,
        ReadWriteExecute = PAGE_EXECUTE_READWRITE,
    };

    class VirtualAddress
    {
    private:
        std::size_t m_nVA{};

    public:
        VirtualAddress() {}
        VirtualAddress(const std::size_t nVA) : m_nVA{ nVA } {}
        VirtualAddress(const void* pMem) : m_nVA{ reinterpret_cast<std::size_t>(pMem) } { }

    public:
        template<typename T>
        auto Ptr(const std::size_t nOffset = 0) const -> T
        {
            return reinterpret_cast<T>(m_nVA + nOffset);
        }

        template<typename T = std::size_t>
        auto VA() const -> T
        {
            return static_cast<T>(m_nVA);
        }

        template<class T, std::size_t S>
        auto Read(const std::size_t nOffset, const std::span<T, S> spBuffer) const -> void
        {
            std::memcpy(spBuffer.data(), this->Ptr<void*>(nOffset), spBuffer.size_bytes());
        }

        template<class T, std::size_t S>
        auto Write(const std::size_t nOffset, const std::span<T, S> spData) const -> void
        {
            std::memcpy(this->Ptr<void*>(nOffset), spData.data(), spData.size_bytes());
        }

        auto Fill(const std::size_t nOffset, const std::uint8_t nValue, const std::size_t nBytes) const -> void
        {
            std::memset(this->Ptr<void*>(nOffset), nValue, nBytes);
        }

        template<typename T>
        auto Get(const std::size_t nOffset) const -> T
        {
            return *this->Ptr<T*>(nOffset);
        }

        template<typename T>
        auto Put(const std::size_t nOffset, const T& rfValue) const -> void
        {
            *this->Ptr<T*>(nOffset) = rfValue;
        }
    };

    class VirtualAllocator
    {
    public:
        static auto Free(const VirtualAddress& rfVA) -> bool;
        static auto Alloc(const std::size_t nBytes, const VirtualProperty eProperty) -> std::pair<VirtualAddress, bool>;
    };

    class VirtualProtector
    {
    public:
        static auto Set(const VirtualAddress& rfVA, const VirtualProperty eProperty, const std::size_t nBytes) -> bool;
    };

    template <bool isBackward>
    auto MemSearch(const std::span<std::uint8_t> spSource, const std::span<std::uint8_t> spMatch) -> const void*
    {
        if (spSource.empty() && spMatch.empty()) { return nullptr; }

        if constexpr (isBackward)
        {
            for (std::size_t ite = 0; ite < spSource.size(); ite++)
            {
                if (::memcmp(spMatch.data(), spSource.data() - ite, spMatch.size()) == 0)
                {
                    return (spSource.data() - ite + 1);
                }
            }
        }
        else
        {
            for (std::size_t ite = 0; ite < spSource.size(); ite++)
            {
                if (::memcmp(spMatch.data(), spSource.data() + ite, spMatch.size()) == 0)
                {
                    return (spSource.data() + ite - 1);
                }
            }
        }

        return nullptr;
    }
} // namespace ZQF::ZxHook
