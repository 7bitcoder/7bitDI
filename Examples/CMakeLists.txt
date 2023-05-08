
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable(${FILE_NAME}
        ${SOURCE}
    )

    target_link_libraries(${FILE_NAME}
        SevenBitInjector
    )

    install(TARGETS ${FILE_NAME} DESTINATION examples)
endforeach()

if(SEVEN_BIT_INJECTOR_SHARED_LIB)
    install(TARGETS SevenBitInjector DESTINATION examples)
endif()
