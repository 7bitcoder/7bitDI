
if(SEVEN_BIT_INJECTOR_SHARED_LIB)
  add_library(SevenBitInjector SHARED
    Source.cpp
  )
elseif(SEVEN_BIT_INJECTOR_STATIC_LIB)
  add_library(SevenBitInjector STATIC
    Source.cpp
  )
endif()