
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach (SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)

    add_executable("${FILE_NAME}Example"
            ${SOURCE}
    )

    target_link_libraries("${FILE_NAME}Example"
            7bitDI
    )
endforeach ()
