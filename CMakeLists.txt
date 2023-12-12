cmake_minimum_required(VERSION 3.15.0)

set(_7BIT_DI_VERSION_MAJOR 1)
set(_7BIT_DI_VERSION_MINOR 0)
set(_7BIT_DI_VERSION_PATCH 0)

set(_7BIT_DI_VERSION ${_7BIT_DI_VERSION_MAJOR}.${_7BIT_DI_VERSION_MINOR}.${_7BIT_DI_VERSION_PATCH})

project(7bitDI LANGUAGES CXX VERSION ${_7BIT_DI_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Cmake")

include(Setup)
include(GNUInstallDirs)


include_directories(${CMAKE_SOURCE_DIR}/Include)

add_subdirectory(Source)

if (_7BIT_DI_BUILD_DOCS)
    add_subdirectory(Docs)
endif ()

if (_7BIT_DI_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()

if (_7BIT_DI_BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif ()

if (_7BIT_DI_BUILD_SINGLE_HEADER)
    add_subdirectory(SingleHeader)
endif ()

if (_7BIT_DI_INSTALL)
    set(PROJECT_CONFIG_IN ${CMAKE_SOURCE_DIR}/Cmake/7bitDIConfig.cmake.in)
    set(PROJECT_CONFIG_OUT ${CMAKE_BINARY_DIR}/7bitDIConfig.cmake)
    set(CONFIG_TARGETS_FILE 7bitDIConfigTargets.cmake)
    set(VERSIONS_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/7bitDIConfigVersion.cmake)
    set(EXPORT_DEST_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/7bitDI)

    install(DIRECTORY ${_7BIT_DI_HEADERS_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(
            TARGETS 7bitDI
            EXPORT 7bitDI
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    export(
            TARGETS 7bitDI
            NAMESPACE 7bitDI::
            FILE ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE})

    install(
            EXPORT 7bitDI
            EXPORT 7bitDI
            DESTINATION ${EXPORT_DEST_DIR}
            NAMESPACE 7bitDI::
            NAMESPACE 7bitDI::
            FILE ${CONFIG_TARGETS_FILE})

    include(CMakePackageConfigHelpers)

    configure_package_config_file(${PROJECT_CONFIG_IN} ${PROJECT_CONFIG_OUT}
            INSTALL_DESTINATION ${EXPORT_DEST_DIR})

    write_basic_package_version_file(${VERSIONS_CONFIG_FILE} COMPATIBILITY SameMajorVersion)

    install(FILES ${PROJECT_CONFIG_OUT} ${VERSIONS_CONFIG_FILE} DESTINATION ${EXPORT_DEST_DIR})

    export(PACKAGE 7bitDI)

    include(CPack)
endif ()
