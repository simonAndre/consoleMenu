// configuration inspired from ArduinoJson - arduinojson.org (Benoit Blanchon 2014-2020)

#pragma once

// #include <consoleMenu/Configuration.hpp>
// #include <consoleMenu/version.hpp>
#include "../version.hpp"

#define CONSOLEMENU_DO_CONCAT(A, B) A##B
#define CONSOLEMENU_CONCAT2(A, B) CONSOLEMENU_DO_CONCAT(A, B)
#define CONSOLEMENU_CONCAT4(A, B, C, D) \
  CONSOLEMENU_CONCAT2(CONSOLEMENU_CONCAT2(A, B), CONSOLEMENU_CONCAT2(C, D))
#define CONSOLEMENU_CONCAT8(A, B, C, D, E, F, G, H)    \
  CONSOLEMENU_CONCAT2(CONSOLEMENU_CONCAT4(A, B, C, D), \
                      CONSOLEMENU_CONCAT4(E, F, G, H))
#define CONSOLEMENU_CONCAT12(A, B, C, D, E, F, G, H, I, J, K, L) \
  CONSOLEMENU_CONCAT8(A, B, C, D, E, F, G,                       \
                      CONSOLEMENU_CONCAT4(H, I, J, CONSOLEMENU_CONCAT2(K, L)))

#define CONSOLEMENU_NAMESPACE                                            \
  CONSOLEMENU_CONCAT12(                                                  \
      consoleMenu, CONSOLEMENU_VERSION_MAJOR, CONSOLEMENU_VERSION_MINOR, \
      CONSOLEMENU_VERSION_REVISION, _, CONSOLEMENU_USE_LONG_LONG,        \
      CONSOLEMENU_USE_DOUBLE, CONSOLEMENU_ENABLE_PROGMEM, , , , )
