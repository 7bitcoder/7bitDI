
include(GenerateExportHeader)

if (_7BIT_DI_SHARED_LIB)
    add_library(7bitDI SHARED Source.cpp)

elseif (_7BIT_DI_STATIC_LIB)
    add_library(7bitDI STATIC Source.cpp)

elseif (_7BIT_DI_HEADER_ONLY_LIB)
    add_library(7bitDI INTERFACE)

endif ()

add_library(7bitDI::7bitDI ALIAS 7bitDI)

set_target_properties(7bitDI PROPERTIES VERSION ${_7BIT_DI_VERSION})
set_target_properties(7bitDI PROPERTIES DEBUG_POSTFIX d)

string(REPLACE ";" "$<SEMICOLON>" dirs "${_7BIT_DI_INCLUDE_DIR}")
target_include_directories(7bitDI SYSTEM INTERFACE
        "$<BUILD_INTERFACE:${dirs}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
