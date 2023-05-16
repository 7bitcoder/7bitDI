
include(GenerateExportHeader)

if(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "Shared")
  add_library(7bitDI SHARED
    Source.cpp ${_7BIT_DI_ALL_HEADERS}
  )
elseif(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "Static")
  add_library(7bitDI STATIC
    Source.cpp ${_7BIT_DI_ALL_HEADERS}
  )
elseif(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "HeaderOnly")
  add_library(7bitDI INTERFACE)
endif()

add_library(7bitDI::7bitDI ALIAS 7bitDI)

target_include_directories(7bitDI INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(7bitDI PROPERTIES VERSION ${_7BIT_DI_VERSION})
set_target_properties(7bitDI PROPERTIES DEBUG_POSTFIX d)

if(_7BIT_DI_INSTALL)
  install(
    TARGETS 7bitDI
    EXPORT 7bitDI)
endif()