find_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
    ${SOURCES}
)

target_link_libraries(Tests PUBLIC
    GTest::gtest
    GTest::gmock
)

if(SEVEN_BIT_DI_BUILD_TESTS_HO)
    target_link_libraries(Tests PUBLIC
        7BitDI_header_only
    )
else()
    target_link_libraries(Tests PUBLIC
        7BitDI
    )
endif()

install(TARGETS Tests)

gtest_discover_tests(Tests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    PROPERTIES TIMEOUT 10)
