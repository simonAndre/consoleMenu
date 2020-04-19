#pragma once

#include "Configuration.h"

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

#include "Menu/Menu.hpp"

#if CONSOLEMENU_EMBEDDED_MODE
#include <Arduino.h>
#endif

// #pragma message("consolemenu EMBEDDED_MODE:" STRING(CONSOLEMENU_EMBEDDED_MODE))

namespace consoleMenu
{
using CONSOLEMENU_NAMESPACE::IoHelpers;
using CONSOLEMENU_NAMESPACE::Menu;
using CONSOLEMENU_NAMESPACE::Menubase;
using CONSOLEMENU_NAMESPACE::Menuitem;
using CONSOLEMENU_NAMESPACE::MenuitemBack;
// using CONSOLEMENU_NAMESPACE::MenuitemCallback;
using CONSOLEMENU_NAMESPACE::MenuitemHierarchy;
using CONSOLEMENU_NAMESPACE::MenuitemUpdater;
using CONSOLEMENU_NAMESPACE::MenuOptions;
using CONSOLEMENU_NAMESPACE::menutype;
using CONSOLEMENU_NAMESPACE::ushort;
} // namespace consoleMenu
