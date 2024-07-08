#include <Windows.h>


INT APIENTRY wWinMain(_In_ HINSTANCE /* hInstance */, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
{
    ::MessageBoxW(nullptr, L"Try hook me!", nullptr, MB_OK);
}
