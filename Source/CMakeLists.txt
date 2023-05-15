
if(SEVEB_BIT_DI_BUILD_SHARED OR BUILD_SHARED_LIBS)
  add_library(SevenBitDI SHARED
    Source.cpp ${SEVEB_BIT_DI_ALL_HEADERS}
  )
  target_compile_definitions(SevenBitDI PUBLIC SEVEB_BIT_DI_BUILD_SHARED)
else()
  add_library(SevenBitDI STATIC
    Source.cpp ${SEVEB_BIT_DI_ALL_HEADERS}
  )
endif()

target_compile_definitions(SevenBitDI PUBLIC SEVEN_BIT_DI_COMPILED_LIB)

target_include_directories(SevenBitDI INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(SevenBitDI PROPERTIES VERSION ${SEVEN_BIT_DI_VERSION})
set_target_properties(SevenBitDI PROPERTIES DEBUG_POSTFIX d)

add_library(SevenBitDI_header_only INTERFACE)

target_include_directories(SevenBitDI_header_only INTERFACE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
  TARGETS SevenBitDI SevenBitDI_header_only
  EXPORT SevenBitDI)
