
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(SOURCE ${SOURCES})
    get_filename_component(FILE_NAME ${SOURCE} NAME_WLE)
    get_filename_component(DIRECTORY_PATH ${SOURCE} DIRECTORY)
    get_filename_component(DIRECTORY_NAME ${DIRECTORY_PATH} NAME)

    string(JOIN "-" EXEC "Example" ${DIRECTORY_NAME} ${FILE_NAME})

    add_executable(${EXEC}
        ${SOURCE}
    )

    target_link_libraries(${EXEC}
        SevenBitRest
    )

    install(TARGETS ${EXEC} DESTINATION Examples)
endforeach()

if(SEVENBITREST_SHARED_LIB)
    install(TARGETS SevenBitRest DESTINATION Examples)
endif()
