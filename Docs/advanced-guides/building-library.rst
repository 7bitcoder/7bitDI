Building Library
==========================

The library can be built locally using Cmake_ (`Cmake Installation`_), and several cache variables can be set:

* <variable cache name>: [possible values] (default value) - Description
* LIBRARY_TYPE: ["Shared", "Static", "HeaderOnly"] ("HeaderOnly") - Library build type
* BUILD_DOCS: [true, false] (false) - Turn on to build documentation (requires Sphinx_, Breathe_ and Doxygen_ installed)
* BUILD_TESTS: [true, false] (false) - Turn on to build tests (requires Gtest_ to be installed, see `Conan Packages Installation`_)
* BUILD_EXAMPLES: [true, false] (false) - Turn on to build examples

build the library using the command:

.. code-block:: sh

    cmake --build ./build


Conan Packages Installation
^^^^^^^^^^^^^^^

Gtest_ library is added to project using Conan_ package manager (`Conan Installation`_), 
to install Conan packages run this command in the library root folder:

.. code-block:: sh

    conan install . --output-folder=build --build=missing

.. _Cmake: https://cmake.org/
.. _`Cmake Installation`: https://cmake.org/download/
.. _Sphinx: https://www.sphinx-doc.org/en/master/
.. _Breathe: https://breathe.readthedocs.io/en/latest/
.. _Doxygen: https://www.doxygen.nl/
.. _Gtest: https://google.github.io/googletest/
.. _Conan: https://conan.io/
.. _`Conan Installation`: https://conan.io/downloads.html