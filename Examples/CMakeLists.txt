
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable(${FILE_NAME}
        ${SOURCE}
    )

    if(SEVEN_BIT_DI_BUILD_EXAMPLE_HO)
        target_link_libraries(${FILE_NAME}
            7BitDI_header_only
        )
    else()
        target_link_libraries(${FILE_NAME}
            7BitDI
        )
    endif()

    install(TARGETS ${FILE_NAME})
endforeach()