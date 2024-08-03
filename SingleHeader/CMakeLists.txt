find_package(quom REQUIRED)

set(_7BIT_DI_SINGLE_IN ${CMAKE_CURRENT_SOURCE_DIR}/SevenBitDI.hpp.in)
set(_7BIT_DI_SINGLE_OUT ${CMAKE_CURRENT_BINARY_DIR}/SevenBitDI.hpp)

add_custom_command(OUTPUT ${_7BIT_DI_SINGLE_OUT}
        COMMAND
        ${QUOM_EXECUTABLE} ${_7BIT_DI_SINGLE_IN} ${_7BIT_DI_SINGLE_OUT} -I ${_7BIT_DI_INCLUDE_DIR}
        COMMENT "Generating single header with Quom")

add_custom_target(GenerateSingleHeader
        ALL DEPENDS
        ${_7BIT_DI_SINGLE_OUT}
)
