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
* _7BIT_DI_LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("Static") - Library build type
* _7BIT_DI_BUILD_DOCS: ["ON", "OFF"] ("OFF") - Turn on to build documentation (requires Sphinx_, Breathe_ and Doxygen_ to be installed)
* _7BIT_DI_BUILD_TESTS: ["ON", "OFF"] ("OFF") - Turn on to build tests
* _7BIT_DI_BUILD_EXAMPLES: ["ON", "OFF"] ("OFF") - Turn on to build examples
* _7BIT_DI_BUILD_BENCHMARKS: ["ON", "OFF"] ("OFF") - Turn on to build benchmarks
* _7BIT_DI_BUILD_SINGLE_HEADER: ["ON", "OFF"] ("OFF") - Turn on to build single header SevenBitDI.hpp (requires Quom_ to be installed)
* _7BIT_DI_INSTALL: ["ON", "OFF"] ("OFF") - Turn on to install the library

to set cache variable pass additional option: -D<variable cache name>=[value] 
for example, this command will set the library type to Static and will force examples built

.. code-block:: sh

    cmake .. -DCMAKE_BUILD_TYPE=Release -D_7BIT_DI_LIBRARY_TYPE=Static -D_7BIT_DI_BUILD_EXAMPLES=true

Build the library using the command:

.. code-block:: sh

    cmake --build .


Install Library
^^^^^^^^^^^^^^^^^^^^^^^^^

To install the library set additional cache variables _7BIT_DI_INSTALL=ON and specify installation dir with CMAKE_INSTALL_PREFIX, then run the command

.. code-block:: sh

    cmake --build . --config Release --target install

.. _Cmake: https://cmake.org/
.. _`Cmake Installation`: https://cmake.org/download/
.. _Sphinx: https://www.sphinx-doc.org/en/master/
.. _Breathe: https://breathe.readthedocs.io/en/latest/
.. _Doxygen: https://www.doxygen.nl/
.. _Gtest: https://google.github.io/googletest/
.. _Conan: https://conan.io/
.. _Quom: https://pypi.org/project/quom/
.. _`Conan Installation`: https://conan.io/downloads.html
