function(centerString OUTPUT STR LEN)
    string(LENGTH "${STR}" STRLEN)
    math(EXPR STRLEN "${LEN} - ${STRLEN}")

    if(STRLEN GREATER 0)
        math(EXPR ADD_ONE "${STRLEN} % 2")
        math(EXPR STRLEN "${STRLEN} / 2")

        string(REPEAT " " ${STRLEN} PAD)

        string(PREPEND STR ${PAD})
        string(APPEND STR ${PAD})

        if(ADD_ONE)
            string(APPEND STR " ")
        endif()
    endif()

    set(${OUTPUT} "${STR}" PARENT_SCOPE)
endfunction()

function(printInfo STRINGS CHAR SIZE HORIZONTAL_SIZE VERTICAL_SIZE)
    string(REPEAT ${CHAR} ${HORIZONTAL_SIZE} HORIZONTAL_SEPARATOR)
    string(REPEAT ${CHAR} ${SIZE} VERTICAL_SEPARATOR)
    string(APPEND VERTICAL_SEPARATOR ${HORIZONTAL_SEPARATOR})
    string(APPEND VERTICAL_SEPARATOR ${HORIZONTAL_SEPARATOR})

    foreach(CNT RANGE ${VERTICAL_SIZE})
        message(STATUS ${VERTICAL_SEPARATOR})
    endforeach()

    foreach(LINE ${STRINGS})
        centerString(CENTERED_LINE ${LINE} ${SIZE})
        message(STATUS "${HORIZONTAL_SEPARATOR}${CENTERED_LINE}${HORIZONTAL_SEPARATOR}")
    endforeach()

    foreach(CNT RANGE ${VERTICAL_SIZE})
        message(STATUS ${VERTICAL_SEPARATOR})
    endforeach()
endfunction()