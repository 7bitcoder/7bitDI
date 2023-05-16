
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable(${FILE_NAME}
        ${SOURCE}
    )

    target_link_libraries(${FILE_NAME}
        7bitDI
    )

    if(_7BIT_DI_INSTALL)
        install(TARGETS ${FILE_NAME})
    endif()
endforeach()