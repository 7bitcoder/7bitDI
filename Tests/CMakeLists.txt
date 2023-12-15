file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
        ${SOURCES}
        Utils/CheckTest.cpp
        Utils/CtorParamsNumberTest.cpp
)

target_link_libraries(Tests PUBLIC
        GTest::gtest
        GTest::gmock
        7bitDI
)

include(GoogleTest)
gtest_discover_tests(Tests
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
