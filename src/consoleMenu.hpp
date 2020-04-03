#pragma once

#include "consoleMenu/Configuration.hpp"

#if !CONSOLEMENU_DEBUG
#ifdef __clang__
#pragma clang system_header
#elif defined __GNUC__
#pragma GCC system_header
#endif
#endif

#include "consoleMenu/Menu/consoleMenu.hpp"

namespace consoleMenu
{

using ARDUINOJSON_NAMESPACE::Menu;
}
