file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(UnitTests
    ${SOURCES}
)

target_link_libraries(UnitTests PUBLIC
    GTest::gtest
    GTest::gmock
    7bitDI
)

include(GoogleTest)
gtest_discover_tests(UnitTests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
