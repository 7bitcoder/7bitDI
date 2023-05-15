#pragma once

#define SEVEN_BIT_DI_VERSION_MAJOR 1
/* #undef SEVEN_BIT_DI_VERSION_MINOR */
/* #undef SEVEN_BIT_DI_VERSION_PATCH */

#define SEVEN_BIT_DI_VERSION "1.0.0"

#ifndef SEVEN_BIT_DI_VERSION_MAJOR
#define SEVEN_BIT_DI_VERSION_MAJOR 0
#endif

#ifndef SEVEN_BIT_DI_VERSION_MINOR
#define SEVEN_BIT_DI_VERSION_MINOR 0
#endif

#ifndef SEVEN_BIT_DI_VERSION_PATCH
#define SEVEN_BIT_DI_VERSION_PATCH 0
#endif

#define SEVEN_BIT_DI_VERSION_AS_NUMBER                                                                                           \
    (SEVEN_BIT_DI_VERSION_MAJOR * 10000 + SEVEN_BIT_DI_VERSION_MINOR * 100 + SEVEN_BIT_DI_VERSION_PATCH)
