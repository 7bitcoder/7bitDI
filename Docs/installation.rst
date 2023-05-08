Installation
==========================

To install 7BitInjector, there are several methods:
----------------

#. Using Conan.io package manager - Recommended
    Info soon...

#. Headder only
    Download source code from the most recent release, 
    copy include folder into your project location,
    for example copy into '/SevenBitInjector' folder. 
    Include this folder into project, with CMake, u can use: 

    .. code-block:: cmake

        include_directories(/SevenBitInjector/include)

#. Building library as Static/Shared
    Download source code from most recent release,
    build project using cmake, set cmake cache var "LIBRARY_TYPE" to "Static" or "Shared",
    run install command 

    .. code-block:: sh

        cmake --build ./build --target install

    library files are located in build/publish folder, include those files into your project, 
    for more details see `Building Library Locally`_ section

Building Library Locally
----------------

Library can be build locally using Cmake_ (`Cmake Installation`_), several cache variables can be set:

* <variable cache name>: [possible values] (default value) - description
* LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("HeaderOnly") - Library build type
* BUILD_DOCS: [true, false] (false) - Turn on to build documentation (requires Sphinx_, Breathe_ and Doxygen_ installed)
* BUILD_TESTS: [true, false] (false) - Turn on to build tests (requires Gtest_ to be installed, see `Conan Packages Installation`_)
* BUILD_EXAMPLES: [true, false] (false) - Turn on to build examples

build library using command:

.. code-block:: sh

    cmake --build ./build


Conan Packages Installation
^^^^^^^^^^^^^^^

Gtest_ library is added to project using Conan_ package manager (`Conan Installation`_), 
to install conan packages run this command in library root folder:

.. code-block:: sh

    conan install . --output-folder=build --build=missing

.. _Cmake: https://cmake.org/
.. _`Cmake Installation`: https://cmake.org/download/
.. _Sphinx: https://www.sphinx-doc.org/en/master/
.. _Breathe: https://breathe.readthedocs.io/en/latest/
.. _Doxygen: https://www.doxygen.nl/
.. _Gtest: https://google.github.io/googletest/
.. _`Conan Installation`: https://conan.io/downloads.html