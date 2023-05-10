Getting Started
==========================

Supported Platforms
--------------------

7bitInjector requires client code and compiler compatible with the C++17 standard or newer.

Library is officially supported on the following platforms:

**Operating systems:**

* Linux
* macOS
* Windows

**Compilers:**

* gcc 5.0+
* clang 5.0+
* MSVC 2015+

If you notice any problems/bugs, please file an issue on the 7bitInjector_ GitHub Issue Tracker. Pull requests containing fixes are welcome!

Installation
--------------------
**There are few ways of installation:**

#. Using Conan.io package manager - Recommended
    Info soon...

#. Headder only
    Download source code from the most recent release, 
    copy include folder into your project location,
    for example copy into '/SevenBitInjector' folder. 
    Include this folder into project, with CMake_, u can use: 

    .. code-block:: cmake

        include_directories(/SevenBitInjector/include)

#. Building library as Static/Shared
    Download source code from most recent release,
    build project using CMake_, set CMake cache var "LIBRARY_TYPE" to "Static" or "Shared",
    run install command 

    .. code-block:: sh

        cmake --build ./build --target install

    library files are located in build/publish folder, include those files into your project, 
    for more details see `Building Library`_ guide

Example Usage
-----------------
Next chapters will in detail, setp by step explain what is going on this example

.. literalinclude:: ../Examples/InjectingServices.cpp
   :caption: Examples/InjectingServices
   :language: C++


.. code-block:: console
   :caption: Output

   actionA, actionB executed.


.. _CMake: https://cmake.org/
.. _7bitInjector: https://github.com/7bitcoder/7bitInjector
.. _`Building Library`: advanced-guides/building-library.html