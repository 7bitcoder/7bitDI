include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_build_tests OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if (_7BIT_DI_BUILD_UNIT_TESTS)
    add_subdirectory(Unit)
endif ()

if (_7BIT_DI_BUILD_INTEGRATION_TESTS)
    add_subdirectory(Integration)
endif ()

if (_7BIT_DI_BUILD_E2E_TESTS)
    add_subdirectory(E2E)
endif ()
