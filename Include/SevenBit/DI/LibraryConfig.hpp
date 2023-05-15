#pragma once

#include "SevenBit/DI/Version.hpp"

#define SEVEN_BIT_DI_HEADER_ONLY

#ifdef SEVEN_BIT_DI_COMPILED_LIB
#undef SEVEN_BIT_DI_HEADER_ONLY

#ifdef SEVEB_BIT_DI_BUILD_SHARED
//!!! cmake when compiling shared lib sets <target_name>_EXPORTS

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
//  Microsoft
#ifdef SevenBitDI_EXPORTS
/* We are building this library */
#define EXPORT __declspec(dllexport)
#else
/* We are using this library */
#define EXPORT __declspec(dllimport)
#endif

#elif defined(__linux__) || defined(UNIX) || defined(__unix__) || defined(LINUX)
//  GCC
#if defined(SevenBitDI_EXPORTS)
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
// Static lib
#define EXPORT
#endif
#endif

#ifdef SEVEN_BIT_DI_HEADER_ONLY

#define SEVEN_BIT_DI_ADD_IMPL
#define INLINE inline
#define EXPORT

#else

#define INLINE

#endif
