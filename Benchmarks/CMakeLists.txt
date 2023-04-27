
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    string(JOIN "-" EXEC "Benchmark" ${FILE_NAME})

    add_executable(${EXEC}
        ${SOURCE}
    )

    target_link_libraries(${EXEC}
        SevenBitRest
        CONAN_PKG::benchmark
    )

    install(TARGETS ${EXEC} DESTINATION Benchmarks)
endforeach()

if(SEVENBITREST_SHARED_LIB)
    install(TARGETS SevenBitRest DESTINATION Benchmarks)
endif()
