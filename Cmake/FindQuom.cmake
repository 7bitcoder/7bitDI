find_program(QUOM_EXECUTABLE
    NAMES quom
    DOC "Path to quom executable")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(quom
    "Failed to find quom executable"
    QUOM_EXECUTABLE)