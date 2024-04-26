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


#. Using Cmake fetch content api - Recommended
    Update CMakeLists.txt file with following code

    .. code-block:: Cmake

        include(FetchContent)
        FetchContent_Declare(
                7bitDI
                GIT_REPOSITORY https://github.com/7bitcoder/7bitDI.git
                GIT_TAG v3.2.0
        )
        FetchContent_MakeAvailable(7bitDI)

        target_link_libraries(Target 7bitDI::7bitDI)

#. Using Conan.io package manager
    Download and install Conan_, and create conanfile.txt in the root of your project for example:

    .. code-block:: Txt

        [requires]
        7bitdi/3.2.0
    
    change the version to newer if available, then run the command:

    .. code-block:: sh

        conan install . --output-folder=build --build=missing

    Follow in detailed instructions available at `Conan Tutorial`_

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


.. _Conan: https://conan.io/
.. _`Conan Tutorial`: https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html
.. _CMake: https://cmake.org/
.. _7bitDI: https://github.com/7bitcoder/7bitDI
.. _`Building Library`: advanced-guides/building-library.html
