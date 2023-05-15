#pragma once

#include "SevenBit/DI/CmakeDef.hpp"
#include "SevenBit/DI/Export.hpp"

#ifndef _7BIT_DI_VERSION

#define _7BIT_DI_VERSION "0.0.0"

#endif

#if !defined _7BIT_DI_SHARED_LIB && !defined _7BIT_DI_STATIC_LIB && !defined _7BIT_DI_HEADER_ONLY_LIB

#define _7BIT_DI_HEADER_ONLY_LIB

#endif

#ifdef _7BIT_DI_HEADER_ONLY_LIB

#define _7BIT_DI_ADD_IMPL
#define INLINE inline

#else

#define INLINE

#endif