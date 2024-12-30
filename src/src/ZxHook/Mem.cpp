#include <ZxHook/Mem.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <cstring>


namespace ZQF::ZxHook
{
    auto VirtualAllocator::Free(const VirtualAddress& rfVA) -> bool
    {
        return ::VirtualFree(rfVA.Ptr<LPVOID>(), 0, MEM_RELEASE) != FALSE;
    }

    auto VirtualAllocator::Alloc(const std::size_t nBytes, const VirtualProperty eProperty) -> std::pair<VirtualAddress, bool>
    {
        const auto va = ::VirtualAlloc(0, nBytes, MEM_COMMIT, static_cast<DWORD>(eProperty));
        return { VirtualAddress{ va }, va != NULL };
    }

    auto VirtualProtector::Set(const VirtualAddress& rfVA, const VirtualProperty eProperty, const std::size_t nBytes) -> bool
    {
        DWORD old_access{};
        return ::VirtualProtect(rfVA.Ptr<LPVOID>(), nBytes, static_cast<DWORD>(eProperty), &old_access) != FALSE;
    }

} // namespace Rut::RxHook
