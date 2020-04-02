#pragma once

#include "consoleMenu/Configuration.hpp"

#if !CONSOLEMENU_DEBUG
#ifdef __clang__
#pragma clang system_header
#elif defined __GNUC__
#pragma GCC system_header
#endif
#endif

#define STRING2(x) #x
#define STRING(x) STRING2(x)
// #pragma message(STRING(__cplusplus))
// #pragma message("consolemenu namespace:" STRING(CONSOLEMENU_NAMESPACE))

#include "consoleMenu/Menu/consoleMenu.hpp"

namespace consoleMenu
{
using CONSOLEMENU_NAMESPACE::Menu;
using CONSOLEMENU_NAMESPACE::MenuOptions;
using CONSOLEMENU_NAMESPACE::pf_callback;
using CONSOLEMENU_NAMESPACE::pf_IOdisplay;
using CONSOLEMENU_NAMESPACE::pf_IOinput;
using CONSOLEMENU_NAMESPACE::ushort;
} // namespace consoleMenu
