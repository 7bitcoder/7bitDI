include(Functions)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)

set(CPACK_PROJECT_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${_7BIT_DI_VERSION})
set(CPACK_PACKAGE_VENDOR "github.com/7bitcoder/7bitDI")
set(CPACK_PACKAGE_CONTACT "https://${CPACK_PACKAGE_VENDOR}")
set(CPACK_PACKAGE_VERSION_MAJOR ${_7BIT_DI_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${_7BIT_DI_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${_7BIT_DI_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "7bitInjector is a simple C++ dependency injection library")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")

set(_7BIT_DI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(_7BIT_DI_DI_DIR "${_7BIT_DI_INCLUDE_DIR}/SevenBit/DI")
set(_7BIT_DI_DETAILS_DIR "${_7BIT_DI_DI_DIR}/Details")

set(_7BIT_DI_MAIN_HEADER "${_7BIT_DI_INCLUDE_DIR}/SevenBit/DI.hpp")
file(GLOB _7BIT_DI_TOP_HEADERS "${_7BIT_DI_DI_DIR}/*.hpp")
file(GLOB _7BIT_DI_DETAILS_HEADERS
        "${_7BIT_DI_DETAILS_DIR}/Containers/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Core/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Factories/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Helpers/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Services/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Utils/*.hpp"
)
file(GLOB _7BIT_DI_IMPL_HEADERS
        "${_7BIT_DI_DI_DIR}/Impl/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Containers/Impl/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Core/Impl/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Factories/Impl/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Helpers/Impl/*.hpp"
        "${_7BIT_DI_DETAILS_DIR}/Utils/Impl/*.hpp"
)
set(_7BIT_DI_ALL_HEADERS ${_7BIT_DI_MAIN_HEADER} ${_7BIT_DI_TOP_HEADERS} ${_7BIT_DI_DETAILS_HEADERS} ${_7BIT_DI_IMPL_HEADERS})

set(_7BIT_DI_LIBRARY_TYPE "Static" CACHE STRING "Library build type: Shared;Static;HeaderOnly")
set(_7BIT_DI_LIBRARY_TYPE_VALUES "Shared;Static;HeaderOnly" CACHE STRING "List of possible _7BIT_DI_LIBRARY_TYPE values")

set_property(CACHE _7BIT_DI_LIBRARY_TYPE PROPERTY STRINGS Shared Static HeaderOnly)

option(_7BIT_DI_BUILD_PIC "Build position independent code (-fPIC)" OFF)
option(_7BIT_DI_BUILD_EXAMPLES "Build example" OFF)
option(_7BIT_DI_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(_7BIT_DI_BUILD_TESTS "Build tests" OFF)
option(_7BIT_DI_BUILD_DOCS "Turn on to build documentation (requires sphinx and doxygen installed)" OFF)
option(_7BIT_DI_INSTALL "Installs 7bitDI" OFF)
option(_7BIT_DI_BUILD_SINGLE_HEADER "Builds single header SevenBitDI.hpp" OFF)

if (_7BIT_DI_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

if (_7BIT_DI_LIBRARY_TYPE STREQUAL "Shared" OR BUILD_SHARED_LIBS)
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "Shared")
    set(_7BIT_DI_SHARED_LIB ON)
elseif (_7BIT_DI_LIBRARY_TYPE STREQUAL "HeaderOnly")
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "HeaderOnly")
    set(_7BIT_DI_HEADER_ONLY_LIB ON)
else ()
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "Static")
    set(_7BIT_DI_STATIC_LIB ON)
endif ()

configure_file(${_7BIT_DI_DI_DIR}/CmakeDef.hpp.input ${_7BIT_DI_DI_DIR}/CmakeDef.hpp)

set(BYTE_SIZE 8)
math(EXPR MEMORY_SIZE "${CMAKE_SIZEOF_VOID_P} * ${BYTE_SIZE}")

set(INFOS
        "${_7BIT_DI_LIBRARY} ${_7BIT_DI_VERSION}"
        "Build type: ${CMAKE_BUILD_TYPE}"
        "Library type: ${_7BIT_DI_BUILD_LIBRARY_TYPE}"
        "=================================================="
        "Cmake version: ${CMAKE_VERSION}"
        "Os: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}"
        "Architecture: ${CMAKE_SYSTEM_PROCESSOR} ${MEMORY_SIZE}bit"
        "CXX compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
        "CXX standard: ${CMAKE_CXX_STANDARD}"
        "Generator: ${CMAKE_GENERATOR}"
        "=================================================="
        "Build tests: ${_7BIT_DI_BUILD_TESTS}"
        "Build examples: ${_7BIT_DI_BUILD_EXAMPLES}"
        "Build benchmarks: ${_7BIT_DI_BUILD_BENCHMARKS}"
        "Build documentation: ${_7BIT_DI_BUILD_DOCS}"
        "Build single header: ${_7BIT_DI_BUILD_SINGLE_HEADER}"
        "Install project: ${_7BIT_DI_INSTALL}"
)
printInfo("${INFOS}" = 50 7 0)
