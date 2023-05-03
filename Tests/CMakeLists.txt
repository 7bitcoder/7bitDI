find_package(GTest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
    ${SOURCES}
)

target_link_libraries(Tests PUBLIC
    GTest::gtest
    GTest::gmock
)

if(NOT SEVEN_BIT_INJECTOR_HEADER_ONLY_LIB)
    target_link_libraries(Tests PUBLIC
        SevenBitInjector
    )
endif()

gtest_discover_tests(Tests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    PROPERTIES TIMEOUT 10)
