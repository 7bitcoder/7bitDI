cmake_minimum_required(VERSION 3.0.0)
project(7bitinjector VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/Cmake)
set(CMAKE_TOOLCHAIN_FILE conan_toolchain.cmake)

include(GoogleTest)
include(CPack)
enable_testing()

include(Setup)

include_directories(Include)

add_subdirectory(Source)

if(BUILD_TESTS)
        add_subdirectory(Tests)
endif()

if(BUILD_BENCHMARKS)
        add_subdirectory(Benchmarks)
endif()

if(BUILD_EXAMPLES)
        add_subdirectory(Examples)
endif()

install(DIRECTORY Include/ DESTINATION include)