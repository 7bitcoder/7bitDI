include(GoogleTest)
enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
    ${SOURCES}
)

target_link_libraries(Tests PUBLIC
    GTest::gtest
    GTest::gmock
)

target_link_libraries(Tests PUBLIC
    7bitDI
)

if(_7BIT_DI_INSTALL)
    install(TARGETS Tests)
endif()

gtest_discover_tests(Tests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    PROPERTIES TIMEOUT 10)
