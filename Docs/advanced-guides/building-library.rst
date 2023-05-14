Build Library
==========================

The library can be built locally using Cmake_ (`Cmake Installation`_)

Create a build directory and navigate to it:

.. code-block:: sh

    mkdir build && cd build

Configure CMake project

.. code-block:: sh

    cmake .. -DCMAKE_BUILD_TYPE=Debug

Using this command several cache variables can be set:

* <variable cache name>: [possible values] (default value) - Description
* LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("HeaderOnly") - Library build type
* BUILD_DOCS: [true, false] (false) - Turn on to build documentation (requires Sphinx_, Breathe_ and Doxygen_ installed)
* BUILD_TESTS: [true, false] (false) - Turn on to build tests (requires Gtest_ to be installed, see `Build Library With Conan`_)
* BUILD_EXAMPLES: [true, false] (false) - Turn on to build examples

to set cache variable pass additional option: -D<variable cache name>=[value] 
for example, this command will set the library type to Static and will force examples built

.. code-block:: sh

    cmake .. -DCMAKE_BUILD_TYPE=Release -DLIBRARY_TYPE=Static -DBUILD_EXAMPLES=true

Build the library using the command:

.. code-block:: sh

    cmake --build .


Build Library With Conan
^^^^^^^^^^^^^^^

Gtest_ library is added to project using Conan_ package manager (`Conan Installation`_), 
If Conan was freshly installed run detect command:

.. code-block:: sh

    conan profile detect

To install Conan packages run this command in the library root folder:

.. code-block:: sh

    conan install . --output-folder=build --build=missing

Navigate to the build directory:

.. code-block:: sh

    cd build

Configure the CMake project, and add also toolchain file as a CMAKE_TOOLCHAIN_FILE cache variable:

.. code-block:: sh

    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=true -DCMAKE_TOOLCHAIN_FILE:STRING="conan_toolchain.cmake"

Build the library using the command:

.. code-block:: sh

    cmake --build .

.. _Cmake: https://cmake.org/
.. _`Cmake Installation`: https://cmake.org/download/
.. _Sphinx: https://www.sphinx-doc.org/en/master/
.. _Breathe: https://breathe.readthedocs.io/en/latest/
.. _Doxygen: https://www.doxygen.nl/
.. _Gtest: https://google.github.io/googletest/
.. _Conan: https://conan.io/
.. _`Conan Installation`: https://conan.io/downloads.html