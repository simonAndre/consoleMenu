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

#include "consoleMenu/Namespace.hpp"
#include "commontypes.hpp"
#include "consoleMenu/Menu/types.hpp"
#include "consoleMenu/Menu/Menuitem.hpp"
#include "consoleMenu/Menu/IOcallbacks.hpp"
#include "consoleMenu/Menu/consoleMenu.hpp"

#if CONSOLEMENU_EMBEDDED_MODE
#include <Arduino.h>
#endif

// #pragma message("consolemenu EMBEDDED_MODE:" STRING(CONSOLEMENU_EMBEDDED_MODE))

namespace consoleMenu
{
// using CONSOLEMENU_NAMESPACE::fp_callback;
// using CONSOLEMENU_NAMESPACE::fp_IOdisplay;
// using CONSOLEMENU_NAMESPACE::fp_IOinput;
using CONSOLEMENU_NAMESPACE::Menu;
using CONSOLEMENU_NAMESPACE::MenuOptions;
using CONSOLEMENU_NAMESPACE::ushort;
} // namespace consoleMenu
