#include <ZxHook/Inject.h>
#include <print>
#include <iostream>


auto main() -> int
{
    try
    {
        ZQF::ZxLoader::ZxCreateProcess(LR"(zxhook-sample-exe.exe)", { "zxhook-sample-dll.dll" });
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
