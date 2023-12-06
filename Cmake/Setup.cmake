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
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")

set(_7BIT_DI_HEADERS_DIR "${CMAKE_SOURCE_DIR}/Include")

set(_7BIT_DI_MAIN_HEADER "${_7BIT_DI_HEADERS_DIR}/SevenBit/DI.hpp")
file(GLOB _7BIT_DI_TOP_HEADERS "${_7BIT_DI_HEADERS_DIR}/SevenBit/DI/*.hpp")
file(GLOB _7BIT_DI_DETAILS_HEADERS "${_7BIT_DI_HEADERS_DIR}/SevenBit/DI/Details/*.hpp")
file(GLOB _7BIT_DI_IMPL_HEADERS "${_7BIT_DI_HEADERS_DIR}/SevenBit/DI/Impl/*.hpp")
set(_7BIT_DI_ALL_HEADERS ${_7BIT_DI_MAIN_HEADER} ${_7BIT_DI_TOP_HEADERS} ${_7BIT_DI_DETAILS_HEADERS} ${_7BIT_DI_IMPL_HEADERS})

source_group("Header Files\\SevenBit" FILES ${_7BIT_DI_TOP_HEADERS})
source_group("Header Files\\SevenBit\\Details" FILES ${_7BIT_DI_DETAILS_HEADERS})
source_group("Header Files\\SevenBit\\Details\\Impl" FILES ${_7BIT_DI_IMPL_HEADERS})

set(_7BIT_DI_LIBRARY_TYPE "Static" CACHE STRING "Library build type: Shared;Static;HeaderOnly")
set(_7BIT_DI_LIBRARY_TYPE_VALUES "Shared;Static;HeaderOnly" CACHE STRING "List of possible _7BIT_DI_LIBRARY_TYPE values")

set_property(CACHE _7BIT_DI_LIBRARY_TYPE PROPERTY STRINGS Shared Static HeaderOnly)

option(_7BIT_DI_BUILD_PIC "Build position independent code (-fPIC)" OFF)
option(_7BIT_DI_BUILD_EXAMPLES "Build example" OFF)
option(_7BIT_DI_BUILD_TESTS "Build tests" OFF)
option(_7BIT_DI_BUILD_DOCS "Turn on to build documentation (requires sphinx and doxygen installed)" OFF)
option(_7BIT_DI_INSTALL "Installs 7bitDI" OFF)
option(_7BIT_DI_BUILD_SINGLE_HEADER "Builds single header SevenBitDI.hpp" OFF)

if (_7BIT_DI_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

if (_7BIT_DI_LIBRARY_TYPE STREQUAL "Shared")
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "Shared")
    set(_7BIT_DI_SHARED_LIB ON)
elseif (_7BIT_DI_LIBRARY_TYPE STREQUAL "HeaderOnly")
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "HeaderOnly")
    set(_7BIT_DI_HEADER_ONLY_LIB ON)
else ()
    set(_7BIT_DI_BUILD_LIBRARY_TYPE "Static")
    set(_7BIT_DI_STATIC_LIB ON)
endif ()

configure_file(${CMAKE_SOURCE_DIR}/Include/SevenBit/DI/CmakeDef.hpp.input ${CMAKE_SOURCE_DIR}/Include/SevenBit/DI/CmakeDef.hpp)

set(INFOS
        "${CMAKE_PROJECT_NAME} version: ${_7BIT_DI_VERSION}"
        "${CMAKE_PROJECT_NAME} build type: ${CMAKE_BUILD_TYPE} "
        "${CMAKE_PROJECT_NAME} build as ${_7BIT_DI_BUILD_LIBRARY_TYPE} library"
        "=================================================="
        "Build tests: ${_7BIT_DI_BUILD_TESTS}"
        "Build examples: ${_7BIT_DI_BUILD_EXAMPLES}"
        "Build documentation: ${_7BIT_DI_BUILD_DOCS}"
        "Build single header: ${_7BIT_DI_BUILD_SINGLE_HEADER}"
        "Install project: ${_7BIT_DI_INSTALL}"
)
printInfo("${INFOS}" = 50 7 0)
