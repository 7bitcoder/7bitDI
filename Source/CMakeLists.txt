
include(GenerateExportHeader)

if(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "Shared")
  add_library(7BitDI SHARED
    Source.cpp ${_7BIT_DI_ALL_HEADERS}
  )
elseif(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "Static")
  add_library(7BitDI STATIC
    Source.cpp ${_7BIT_DI_ALL_HEADERS}
  )
elseif(_7BIT_DI_BUILD_LIBRARY_TYPE STREQUAL "HeaderOnly")
  add_library(7BitDI INTERFACE)
endif()

add_library(7BitDI::7BitDI ALIAS 7BitDI)

target_include_directories(7BitDI INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(7BitDI PROPERTIES VERSION ${_7BIT_DI_VERSION})
set_target_properties(7BitDI PROPERTIES DEBUG_POSTFIX d)

if(_7BIT_DI_INSTALL)
  install(
    TARGETS 7BitDI
    EXPORT 7BitDI)
endif()