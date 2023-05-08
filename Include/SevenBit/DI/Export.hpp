#pragma once

#include "SevenBit/DI/CmakeDef.hpp"

#ifdef SEVEN_BIT_INJECTOR_SHARED_LIB
//!!! cmake when compiling shared lib sets <target_name>_EXPORTS

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
//  Microsoft
#if defined(SevenBitInjector_EXPORTS)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#elif defined(__linux__) || defined(UNIX) || defined(__unix__) || defined(LINUX)
//  GCC
#if defined(SevenBitInjector_EXPORTS)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#endif

#else
// do nothing and hope for the best?
#define EXPORT
#pragma WARNING : Unknown dynamic link import / export semantics.
#endif

#else
// Static or header only lib
#define EXPORT
#endif
