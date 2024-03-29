include(FetchContent)
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
FetchContent_MakeAvailable(benchmark)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach (SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable(${FILE_NAME}
            ${SOURCE}
    )

    target_link_libraries(${FILE_NAME}
            7bitDI
            benchmark::benchmark
    )
endforeach ()
