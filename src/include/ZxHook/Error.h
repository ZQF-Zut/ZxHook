#pragma once


namespace ZQF::ZxHook
{
    auto SysErrorMsgBox(const char* cpText, bool isExit) -> void;
    auto SysErrorMsgBox(const wchar_t* wpText, bool isExit) -> void;
}
