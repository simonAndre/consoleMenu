// configuration inspired from CONSOLEMENU - CONSOLEMENU.org (Benoit Blanchon 2014-2020)

#pragma once

// Small or big machine?
#ifndef CONSOLEMENU_EMBEDDED_MODE
#if defined(ARDUINO)                /* Arduino*/                 \
    || defined(__IAR_SYSTEMS_ICC__) /* IAR Embedded Workbench */ \
    || defined(__XC)                /* MPLAB XC compiler */      \
    || defined(__ARMCC_VERSION)     /* Keil ARM Compiler */      \
    || defined(__AVR)               /* Atmel AVR8/GNU C Compiler */
#define CONSOLEMENU_EMBEDDED_MODE 1
#else
#define CONSOLEMENU_EMBEDDED_MODE 0
#endif
#endif

// Auto enable std::string if the right header is here and no conflicting
// macro is defined
#if !defined(CONSOLEMENU_ENABLE_STD_STRING) && defined(__has_include)
#if __has_include(<string>) && !defined(min) && !defined(max)
#define CONSOLEMENU_ENABLE_STD_STRING 1
#else
#define CONSOLEMENU_ENABLE_STD_STRING 0
#endif
#endif

#if CONSOLEMENU_EMBEDDED_MODE

// Store floats by default to reduce the memory usage (issue #134)
#ifndef CONSOLEMENU_USE_DOUBLE
#define CONSOLEMENU_USE_DOUBLE 0
#endif

// Store longs by default, because they usually match the size of a float.
#ifndef CONSOLEMENU_USE_LONG_LONG
#define CONSOLEMENU_USE_LONG_LONG 0
#endif

// Embedded systems usually don't have std::string
#ifndef CONSOLEMENU_ENABLE_STD_STRING
#define CONSOLEMENU_ENABLE_STD_STRING 0
#endif

// Embedded systems usually don't have std::stream
#ifndef CONSOLEMENU_ENABLE_STD_STREAM
#define CONSOLEMENU_ENABLE_STD_STREAM 0
#endif

// Limit nesting as the stack is likely to be small
#ifndef CONSOLEMENU_DEFAULT_NESTING_LIMIT
#define CONSOLEMENU_DEFAULT_NESTING_LIMIT 10
#endif

#else // CONSOLEMENU_EMBEDDED_MODE

// On a computer we have plenty of memory so we can use doubles
#ifndef CONSOLEMENU_USE_DOUBLE
#define CONSOLEMENU_USE_DOUBLE 1
#endif

// Use long long when available
#ifndef CONSOLEMENU_USE_LONG_LONG
#if CONSOLEMENU_HAS_LONG_LONG || CONSOLEMENU_HAS_INT64
#define CONSOLEMENU_USE_LONG_LONG 1
#else
#define CONSOLEMENU_USE_LONG_LONG 0
#endif
#endif

// On a computer, we can use std::string
#ifndef CONSOLEMENU_ENABLE_STD_STRING
#define CONSOLEMENU_ENABLE_STD_STRING 1
#endif

// On a computer, we can assume std::stream
#ifndef CONSOLEMENU_ENABLE_STD_STREAM
#define CONSOLEMENU_ENABLE_STD_STREAM 1
#endif

// On a computer, the stack is large so we can increase nesting limit
#ifndef CONSOLEMENU_DEFAULT_NESTING_LIMIT
#define CONSOLEMENU_DEFAULT_NESTING_LIMIT 50
#endif

#endif // CONSOLEMENU_EMBEDDED_MODE

#ifdef ARDUINO

#include <Arduino.h>

// Enable support for Arduino's String class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_STRING
#define CONSOLEMENU_ENABLE_ARDUINO_STRING 1
#endif

// Enable support for Arduino's Stream class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_STREAM
#define CONSOLEMENU_ENABLE_ARDUINO_STREAM 1
#endif

// Enable support for Arduino's Print class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_PRINT
#define CONSOLEMENU_ENABLE_ARDUINO_PRINT 1
#endif

#else // ARDUINO

// Disable support for Arduino's String class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_STRING
#define CONSOLEMENU_ENABLE_ARDUINO_STRING 0
#endif

// Disable support for Arduino's Stream class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_STREAM
#define CONSOLEMENU_ENABLE_ARDUINO_STREAM 0
#endif

// Disable support for Arduino's Print class
#ifndef CONSOLEMENU_ENABLE_ARDUINO_PRINT
#define CONSOLEMENU_ENABLE_ARDUINO_PRINT 0
#endif

#endif // ARDUINO

#ifndef CONSOLEMENU_ENABLE_PROGMEM
#ifdef PROGMEM
#define CONSOLEMENU_ENABLE_PROGMEM 1
#else
#define CONSOLEMENU_ENABLE_PROGMEM 0
#endif
#endif

#ifndef CONSOLEMENU_TAB
#define CONSOLEMENU_TAB "  "
#endif

#ifndef CONSOLEMENU_STRING_BUFFER_SIZE
#define CONSOLEMENU_STRING_BUFFER_SIZE 32
#endif

#ifndef CONSOLEMENU_DEBUG
#ifdef __PLATFORMIO_BUILD_DEBUG__
#define CONSOLEMENU_DEBUG 1
#else
#define CONSOLEMENU_DEBUG 0
#endif
#endif
