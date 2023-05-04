
if(LIBRARY_TYPE STREQUAL "Shared")
  add_library(SevenBitInjector SHARED
    Source.cpp
  )
elseif(LIBRARY_TYPE STREQUAL "Static")
  add_library(SevenBitInjector STATIC
    Source.cpp
  )
elseif(LIBRARY_TYPE STREQUAL "HeaderOnly")
  add_library(SevenBitInjector INTERFACE)
endif()

target_include_directories(SevenBitInjector INTERFACE ../Include)

install(TARGETS SevenBitInjector)