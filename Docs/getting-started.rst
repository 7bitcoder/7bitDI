Getting Started
==========================

Supported Platforms
--------------------

7bitDI requires client code and compiler compatible with the C++17 standard or newer.

The library is officially supported on the following platforms:

**Operating systems:**

* Linux
* macOS
* Windows

**Compilers:**

* gcc 7.0+
* clang 6.0+
* MSVC 2015+

If you notice any problems/bugs, please file an issue on the 7bitDI_ GitHub Issue Tracker. Pull requests containing fixes are welcome!

Installation
--------------------
**There are a few ways of installation:**

#. Using Conan.io package manager - Recommended
    Info soon...

#. Header only
    Download source code from the most recent release, 
    copy include folder into your project location,
    for example copy into the '/SevenBitDI' folder. 
    Include this folder into the project, with CMake_, u can use: 

    .. code-block:: CMake

        include_directories(/SevenBitDI/Include)

#. Header only - Single file
    Download SevenBitDI.hpp header file from the most recent release, 
    copy this file into your project location and include it.

#. Building library as Static/Shared
    Download source code from the most recent release, build or install the project using CMake_,
    library files are located in the build/publish folder,
    for more details see the `Building Library`_ guide.

Example Usage
-----------------
The next chapters will in detail, step by step explain what is going on in this example

.. literalinclude:: ../Examples/Guides/InjectingServices.cpp
   :caption: Examples/Guides/InjectingServices
   :language: C++


.. code-block:: console
   :caption: Output

   actionA, actionB executed.


.. _CMake: https://cmake.org/
.. _7bitDI: https://github.com/7bitcoder/7bitDI
.. _`Building Library`: advanced-guides/building-library.html