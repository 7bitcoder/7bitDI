cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/Cmake)

include(Setup)

project(${SEVEN_BIT_DI_PROJECT_NAME} VERSION ${SEVEN_BIT_DI_VERSION})

include_directories(Include)

add_subdirectory(Source)

if(SEVEN_BIT_DI_BUILD_DOCS)
        add_subdirectory(Docs)
endif()

if(SEVEN_BIT_DI_BUILD_TESTS OR SEVEN_BIT_DI_BUILD_TESTS_HO)
        include(GoogleTest)
        include(CPack)
        enable_testing()

        add_subdirectory(Tests)
endif()

if(SEVEN_BIT_DI_BUILD_EXAMPLES OR SEVEN_BIT_DI_BUILD_EXAMPLE_HO)
        add_subdirectory(Examples)
endif()

install(DIRECTORY ${SEVEN_BIT_DI_HEADERS_DIR}/ DESTINATION include)

export(
        TARGETS 7BitDI
        NAMESPACE 7BitDI::
        FILE ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE})
install(EXPORT 7BitDI DESTINATION ${EXPORT_DEST_DIR} NAMESPACE 7BitDI:: FILE ${CONFIG_TARGETS_FILE})

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_CONFIG_IN} ${PROJECT_CONFIG_OUT}
        INSTALL_DESTINATION ${EXPORT_DEST_DIR})

write_basic_package_version_file(${VERSIONS_CONFIG_FILE} COMPATIBILITY SameMajorVersion)
install(FILES ${PROJECT_CONFIG_OUT} ${VERSIONS_CONFIG_FILE} DESTINATION ${EXPORT_DEST_DIR})
