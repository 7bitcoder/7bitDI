find_package(Sphinx REQUIRED)

file(GLOB_RECURSE EXAMPLES ../Examples/*.hpp)
file(GLOB_RECURSE SHPHINX_RST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.rst)
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx/${SEVEN_BIT_INJECTOR_VERSION})
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        ${SPHINX_EXECUTABLE} -b html
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${SPHINX_SOURCE}
        DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
        ${SHPHINX_RST_FILES}
        ${EXAMPLES}
        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
        COMMENT "Generating documentation with Sphinx")

add_custom_target(GenerateDoc
        ALL DEPENDS
        ${SPHINX_INDEX_FILE}
        ${SHPHINX_RST_FILES}
)

include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})