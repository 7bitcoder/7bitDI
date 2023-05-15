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
