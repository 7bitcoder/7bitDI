
if (_7BIT_DI_BUILD_EXAMPLES)
    find_package(Python REQUIRED)

    add_test(NAME E2E.Examples COMMAND ${Python_EXECUTABLE} test_examples.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Examples)
endif ()
