find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

file(GLOB_RECURSE PUBLIC_HEADERS ../Include/*.hpp)
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/Include)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        DEPENDS ${PUBLIC_HEADERS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating xml documentation with doxygen" VERBATIM)

add_custom_target(GenerateXmlDoc ALL DEPENDS ${DOXYGEN_INDEX_FILE})

file(GLOB_RECURSE EXAMPLES ../Examples/*.hpp)
file(GLOB_RECURSE SHPHINX_RST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.rst)
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        ${SPHINX_EXECUTABLE} -b html
        -Dbreathe_projects.7BitInjector=${DOXYGEN_OUTPUT_DIR}/xml
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
        ${DOXYGEN_INDEX_FILE}
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