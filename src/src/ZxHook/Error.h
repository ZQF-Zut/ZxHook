#pragma once


namespace ZQF::ZxHook
{
    auto SysErrorMsgBox(const char* cpText, const bool isExit) -> void;
    auto SysErrorMsgBox(const wchar_t* wpText, const bool isExit) -> void;
}
