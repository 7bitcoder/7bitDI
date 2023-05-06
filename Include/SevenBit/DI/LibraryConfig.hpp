#pragma once

#include "SevenBit/DI/CmakeDef.hpp"

#ifndef SEVEN_BIT_INJECTOR_VERSION

#define SEVEN_BIT_INJECTOR_VERSION "0.0.0"

#endif

#if !defined SEVEN_BIT_INJECTOR_SHARED_LIB && !defined SEVEN_BIT_INJECTOR_STATIC_LIB &&                                \
    !defined SEVEN_BIT_INJECTOR_HEADER_ONLY_LIB

#define SEVEN_BIT_INJECTOR_HEADER_ONLY_LIB

#endif

#ifdef SEVEN_BIT_INJECTOR_HEADER_ONLY_LIB

#define SEVEN_BIT_INJECTOR_ADD_IMPL
#define INLINE inline

#else

#define INLINE

#endif
