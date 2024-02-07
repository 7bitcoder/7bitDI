file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(IntegrationTests
    ${SOURCES}
)

target_link_libraries(IntegrationTests PUBLIC
    GTest::gtest
    GTest::gmock
    7bitDI
)

include(GoogleTest)
gtest_discover_tests(IntegrationTests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
