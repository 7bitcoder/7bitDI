include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

enable_testing()

add_executable(Tests
        ${SOURCES}
)

target_link_libraries(Tests PUBLIC
        GTest::gtest
        GTest::gmock
        7bitDI
)

include(GoogleTest)
gtest_discover_tests(Tests
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
