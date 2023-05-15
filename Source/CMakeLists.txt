
if(SEVEB_BIT_DI_BUILD_SHARED OR BUILD_SHARED_LIBS)
  add_library(7BitDI SHARED
    Source.cpp ${SEVEB_BIT_DI_ALL_HEADERS}
  )
  target_compile_definitions(7BitDI PUBLIC SEVEB_BIT_DI_BUILD_SHARED)
else()
  add_library(7BitDI STATIC
    Source.cpp ${SEVEB_BIT_DI_ALL_HEADERS}
  )
endif()

target_compile_definitions(7BitDI PUBLIC SEVEN_BIT_DI_COMPILED_LIB)

target_include_directories(7BitDI INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(7BitDI PROPERTIES VERSION ${SEVEN_BIT_DI_VERSION})
set_target_properties(7BitDI PROPERTIES DEBUG_POSTFIX d)

add_library(7BitDI_header_only INTERFACE)

target_include_directories(7BitDI_header_only INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
  TARGETS 7BitDI 7BitDI_header_only
  EXPORT 7BitDI)
