[![DevCI](https://github.com/7bitcoder/7bitDI/actions/workflows/DevCI.yml/badge.svg?branch=dev)](https://github.com/7bitcoder/7bitDI/actions/workflows/DevCI.yml)
[![Linux](https://github.com/7bitcoder/7bitDI/actions/workflows/Linux.yml/badge.svg)](https://github.com/7bitcoder/7bitDI/actions/workflows/Linux.yml)
[![Windows](https://github.com/7bitcoder/7bitDI/actions/workflows/Windows.yml/badge.svg)](https://github.com/7bitcoder/7bitDI/actions/workflows/Windows.yml)
[![MacOs](https://github.com/7bitcoder/7bitDI/actions/workflows/MacOs.yml/badge.svg)](https://github.com/7bitcoder/7bitDI/actions/workflows/MacOs.yml)
<div align="center">

  <img src="Docs/_static/logo.svg" alt="logo" width="500" height="auto" />

  <h4>
    C++17 simple dependency injection library!
  </h4>

  <p>
    7bitDI is a simple C++ dependency injection library, designed to be as easy to use as possible, the main inspiration was the asp net core dependency injection system. 
  </p>

  <h4>
    <a href="https://7bitDI.readthedocs.io/en/latest/index.html">Documentation & Examples</a>
  </h4>
</div>

<br />

## Built With

- [Google Test](https://github.com/google/googletest)

## Supported Platforms

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

If you notice any problems/bugs, please file an issue on the repository GitHub Issue Tracker. Pull requests containing
fixes are welcome!

## Installation

### There are a few ways of installation:

### 1. Using Cmake fetch content api - Recommended

Update CMakeLists.txt file with following code

```cmake
include(FetchContent)
FetchContent_Declare(
        7bitDI
        GIT_REPOSITORY https://github.com/7bitcoder/7bitDI.git
        GIT_TAG 86228173f14f449dde88a84c549474ba43c2fd25 # proper release tag for example 1.0.0
)
FetchContent_MakeAvailable(7bitDI)
```

### 2. Using Conan.io package manager

Download and install A [Conan](https://conan.io/), and create conanfile.txt in the root of your project for example:

```txt
[requires]
7bitdi/2.0.0
```

change the version to newer if available, then run the command:

```console
conan install . --output-folder=build --build=missing
```

### 3. Header only

Download source code from the most recent release,
copy include folder into your project location,
for example copy into the '/SevenBitDI' folder.
Include this folder into the project, with [Cmake](https://cmake.org/), u can use:

```cmake
include_directories(/SevenBitDI/Include)
```

### 4. Header only - Single file

Download SevenBitDI.hpp header file from the most recent release,
copy this file into your project location and include it.

### 5. Building library as Static/Shared

Download source code from the most recent release, build or install the project using [Cmake](https://cmake.org/),
for more details see the Building Library guide
in [Documentation](https://7bitdi.readthedocs.io/en/latest/getting-started.html).

### The library relies on two core classes:

* ServiceCollection: class is responsible for registering services and building service provider
* ServiceProvider: class is responsible for delivering real services and managing its lifetime

## Injection Rules

The dependency injection system relies heavily on template metaprogramming and it has some limitations.

### General

* Only one constructor should be defined for each instance implementation
* If the service is registered with interface and implementation, the interface should have a virtual destructor
* If multiple services are registered by the same interface, all should have the same lifetime singleton, scoped or
  transient (the build method will throw an exception)
* Only one service implementation can be registered (the build method will throw an exception)

### Service lifetime

Service can be registered as a singleton, scoped, or transient.

* Singleton: service provider will create only one instance of this service (accessible via the getService
  method)
* Scoped: instance provider will create only one instance of this instance for each scope (accessible via the getService
  method)
* Transient: services are always unique, a new service is provided every time it is requested, and the service provider
  returns, in this case, std::unique_ptr (accessible via createService method)

### Injecting Services

* Singleton/scoped services can be injected using one of:
    * References: (T&)
    * Const references: (const T&)
    * Pointers: (T*)
    * Const pointer: (T* const)
    * Pointer to const object: (const T*)
    * Const pointer to const object: (const T* const)
* Transient services can be injected using one of:
    * std::unique_ptr: (unique_ptr< T>)
    * In place object if type is movable or copyable: T
* Multiple services implementing specified interface can be injected using std::vector:
    * Transient (std::vector<std::unique_ptr < T>>)
    * Singleton/scoped (std::vector<T *>)

### Injection Table

| Constructor param type           | ServiceProvider method used         |
|----------------------------------|-------------------------------------|
| T - if movable or copyable       | provider.createServiceInPlace< T>() |
| std::unique_ptr< T>              | provider.createService< T>()        |
| T&                               | provider.getService< T>()           |
| T*                               | provider.tryGetService< T>()        |
| std::vector<T*>                  | provider.getServices< T>()          |
| std::vector<std::unique_ptr< T>> | provider.createServices< T>()       |

### Sample Usage

```cpp
#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

struct IServiceA
{
    virtual std::string actionA() = 0;

    virtual ~IServiceA() = default;
};

struct IServiceB
{
    virtual std::string actionB() = 0;

    virtual ~IServiceB() = default;
};

struct ServiceA final : IServiceA
{
    std::string actionA() override { return "actionA"; }
};

struct ServiceB final : IServiceB
{
    std::string actionB() override { return "actionB"; }
};

class ServiceExecutor
{
    IServiceA &_serviceA;
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ServiceExecutor(IServiceA &serviceA, std::unique_ptr<IServiceB> serviceB)
        : _serviceA(serviceA), _serviceB(std::move(serviceB))
    {
    }

    [[nodiscard]] std::string execute() const
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};
int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IServiceA, ServiceA>()
                                   .addTransient<IServiceB, ServiceB>()
                                   .addScoped<ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<ServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
```

Output

```console
actionA, actionB executed.
```

More examples and tutorials are available on the
[Documentation & Examples](https://7bitDI.readthedocs.io/en/latest/index.html) page

@7bitcoder Sylwester Dawida 2023
