file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
        ${SOURCES}
)

target_link_libraries(Tests PUBLIC
        GTest::gtest
        GTest::gmock
        7bitDI
)

gtest_discover_tests(Tests
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
