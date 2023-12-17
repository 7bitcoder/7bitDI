include(FetchContent)
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG 344117638c8ff7e239044fd0fa7085839fc03021 # release-1.14.0
)
FetchContent_MakeAvailable(benchmark)

enable_testing()

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
