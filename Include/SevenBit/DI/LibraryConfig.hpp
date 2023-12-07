#pragma once

#include "SevenBit/DI/CmakeDef.hpp"

#ifndef _7BIT_DI_VERSION_MAJOR
#define _7BIT_DI_VERSION_MAJOR 0
#endif

#ifndef _7BIT_DI_VERSION_MINOR
#define _7BIT_DI_VERSION_MINOR 0
#endif

#ifndef _7BIT_DI_VERSION_PATCH
#define _7BIT_DI_VERSION_PATCH 0
#endif

#define _7BIT_DI_VERSION_AS_NUMBER                                                                                     \
    (_7BIT_DI_VERSION_MAJOR * 10000 + _7BIT_DI_VERSION_MINOR * 100 + _7BIT_DI_VERSION_PATCH)

#define _7BIT_DI_MACRO_TO_STRING(macro) _7BIT_DI_TO_STRING(macro)
#define _7BIT_DI_TO_STRING(value) #value

#define _7BIT_DI_VERSION                                                                                               \
    _7BIT_DI_MACRO_TO_STRING(_7BIT_DI_VERSION_MAJOR)                                                                   \
    "." _7BIT_DI_MACRO_TO_STRING(_7BIT_DI_VERSION_MINOR) "." _7BIT_DI_MACRO_TO_STRING(_7BIT_DI_VERSION_PATCH)

#if defined _7BIT_DI_STATIC_LIB

#undef _7BIT_DI_SHARED_LIB
#undef _7BIT_DI_HEADER_ONLY_LIB

#define INLINE

#elif defined _7BIT_DI_SHARED_LIB

#undef _7BIT_DI_STATIC_LIB
#undef _7BIT_DI_HEADER_ONLY_LIB

#define INLINE

#else

#define _7BIT_DI_HEADER_ONLY_LIB

#undef _7BIT_DI_SHARED_LIB
#undef _7BIT_DI_STATIC_LIB

#define _7BIT_DI_ADD_IMPL
#define INLINE inline

#endif

#ifndef _7_BIT_DI_CTOR_LIMIT_SIZE
#define _7_BIT_DI_CTOR_LIMIT_SIZE 10
#endif

#include "SevenBit/DI/Export.hpp"
