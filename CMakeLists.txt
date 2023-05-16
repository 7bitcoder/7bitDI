cmake_minimum_required(VERSION 3.15.0)

set(_7BIT_DI_VERSION_MAJOR 1)
set(_7BIT_DI_VERSION_MINOR 0)
set(_7BIT_DI_VERSION_PATCH 0)

set(_7BIT_DI_VERSION ${_7BIT_DI_VERSION_MAJOR}.${_7BIT_DI_VERSION_MINOR}.${_7BIT_DI_VERSION_PATCH})

project(7bitDI LANGUAGES CXX VERSION ${_7BIT_DI_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Cmake")

include(Setup)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/Include)

add_subdirectory(Source)

if(_7BIT_DI_BUILD_DOCS)
        add_subdirectory(Docs)
endif()

if(_7BIT_DI_BUILD_TESTS)
        add_subdirectory(Tests)
endif()

if(_7BIT_DI_BUILD_EXAMPLES)
        add_subdirectory(Examples)
endif()

if(_7BIT_DI_BUILD_SINGLE_HEADER)
        add_subdirectory(SingleHeader)
endif()

if(_7BIT_DI_INSTALL)
        install(DIRECTORY ${_7BIT_DI_HEADERS_DIR}/ DESTINATION include)

        export(
                TARGETS 7BitDI
                NAMESPACE 7BitDI::
                FILE ${CONFIG_TARGETS_FILE})

        install(
                EXPORT 7BitDI
                DESTINATION ${EXPORT_DEST_DIR}
                NAMESPACE 7BitDI::
                FILE ${CONFIG_TARGETS_FILE})

        include(CMakePackageConfigHelpers)

        configure_package_config_file(${PROJECT_CONFIG_IN} ${PROJECT_CONFIG_OUT}
                INSTALL_DESTINATION ${EXPORT_DEST_DIR})

        write_basic_package_version_file(${VERSIONS_CONFIG_FILE} COMPATIBILITY SameMajorVersion)

        install(FILES ${PROJECT_CONFIG_OUT} ${VERSIONS_CONFIG_FILE} DESTINATION ${EXPORT_DEST_DIR})

        export(PACKAGE 7bitDI)

        include(CPack)
endif()