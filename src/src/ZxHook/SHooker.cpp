#include <ZxHook/SHooker.h>
#include <ZxHook/Hook.h>


namespace ZQF::ZxHook
{
    auto SHookerDetour::CommitBeg() -> void
    {
        ZxHook::Detours::Begin();
    }

    auto SHookerDetour::CommitEnd() -> void
    {
        ZxHook::Detours::Commit();
    }

    auto SHookerDetour::PushAttach(void* ppFunc, void* pDetour) -> void
    {
        ZxHook::Detours::Attach(ppFunc, pDetour);
    }

    auto SHookerDetour::PushDetach(void* ppFunc, void* pDetour) -> void
    {
        ZxHook::Detours::Detach(ppFunc, pDetour);
    }

    auto SHookerDetour::AfterWith() -> void
    {
        ZxHook::Detours::AfterWith();
    }
} // namespace ZQF::ZxHook
