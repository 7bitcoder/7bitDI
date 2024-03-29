
if (_7BIT_DI_BUILD_EXAMPLES)
    find_package(Python REQUIRED)

    add_test(NAME E2E.Examples COMMAND ${Python_EXECUTABLE}
            test_examples.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} expected_test_data.json
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Examples)
endif ()


add_executable(BigLoadTest BigLoadTest.cpp)
target_link_libraries(BigLoadTest PUBLIC 7bitDI)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND WIN32)
    target_compile_options(BigLoadTest PRIVATE -Wa -mbig-obj)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(BigLoadTest PRIVATE /bigobj)
endif ()

add_test(NAME BigLoadTest COMMAND BigLoadTest)
