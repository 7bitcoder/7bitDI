find_package(GTest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(Tests
    ${SOURCES}
)

target_link_libraries(Tests PUBLIC
    GTest::gtest
)

gtest_discover_tests(Tests
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    PROPERTIES TIMEOUT 10)

install(TARGETS Tests DESTINATION Tests)

if(SEVENBITREST_SHARED_LIB)
    install(TARGETS Tests DESTINATION Tests)
endif()