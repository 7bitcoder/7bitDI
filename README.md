[![Ubuntu](https://github.com/7bitcoder/SevenBitRest/actions/workflows/Ubuntu.yml/badge.svg?branch=main)](https://github.com/7bitcoder/SevenBitRest/actions/workflows/Ubuntu.yml)
[![Windows](https://github.com/7bitcoder/SevenBitRest/actions/workflows/Windows.yml/badge.svg?branch=main)](https://github.com/7bitcoder/SevenBitRest/actions/workflows/Windows.yml)
[![MacOs](https://github.com/7bitcoder/SevenBitRest/actions/workflows/MacOs.yml/badge.svg?branch=main)](https://github.com/7bitcoder/SevenBitRest/actions/workflows/MacOs.yml)

<div align="center">

  <img src="7bitrest - logo.svg" alt="logo" width="500" height="auto" />
  <p>
    C++20 simple http rest framework! 
  </p>
   
<h4>
    <a href="https://github.com/7bitcoder/SevenBitRest/">Home</a>
  <span> · </span>
    <a href="https://github.com/7bitcoder/SevenBitRest/tree/main/Docs">Documentation</a>
  <span> · </span>
    <a href="https://github.com/7bitcoder/SevenBitRest/issues/">Report Bug</a>
  <span> · </span>
    <a href="https://github.com/7bitcoder/SevenBitRest/issues/">Request Feature</a>
  </h4>
</div>

<br />
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#build-framework-locally">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a>
      <ul>
        <li><a href="#hello-world">Hello World</a></li>
        <li><a href="#dependency-injector">Dependency Injector</a></li>
        <li><a href="#middlewares">Middlewares</a></li>
      </ul>
    </li>
    <li><a href="#prebuild-binaries">Prebuild Binaries</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
</details>

## About The Project

SevenBitRest is a simple C++ http rest framework, designed to be as easy to use as possible, the main inspiration was asp net core minimal api.

### Built With

* [Boost Beast](https://github.com/boostorg/beast)
* [Boost Url](https://github.com/boostorg/url)
* [Taocpp Json](https://github.com/taocpp/json)
* [cli11](https://github.com/CLIUtils/CLI11)
  
#### Testing Utilities
* [Google Test](https://github.com/google/googletest)
* [Google Banchmark](https://github.com/google/benchmark)
* [Curl](https://github.com/curl/curl)

## Getting Started

To use library download prebuild binaries and link dynamic library, or build project locally 

### Prerequisites

Install Cmake if not already installed:

https://cmake.org/

Install Conan:

https://conan.io/

### Build Framework Locally

Framework uses BoostUrl library, it is not yet part of official boost library so it must be installed separately (clone git repo in BoostUrl directory), also to build project benchmarks/examples/tests cmake cache flags should be set (using cmake-gui or in cli):

* Benchmarks - BUILD_BENCHMARKS
* Examples - BUILD_EXAMPLES
* Tests - BUILD_TESTS

#### Prepare

1. Clone the repo
    ```sh
    git clone https://github.com/7bitcoder/SevenBitRest.git
    ```

#### Install Conan Packages

1. Create and navigate to build directory
    ```sh
    mkdir build && cd ./build
    ```
3. Install conan packages
    ```sh
    conan install --build=missing ..
    ```
4. Navigate up
    ```sh
    cd ..
    ```

#### Install BoostUrl Library

1. Create and navigate to BoostUrl directory
    ```sh
    mkdir BoostUrl && cd ./BoostUrl
    ```
2. Clone BoostUrl repository
    ```sh
    git clone https://github.com/boostorg/url.git
    ```
3. Navigate up
    ```sh
    cd ..
    ```

#### Build Framework

1.  Configure project
    ```sh
    cmake -B ./build 
    ```
2.  Build project
    ```sh
    cmake --build ./build
    ```

## Usage

Framework uses asp net core minimal api approach, to build endpoints use Map functions on SevenBitRest object. If not specified app will run on 9090 port by default.

_For more examples, please refer to the [Documentation](https://github.com/7bitcoder/SevenBitRest/tree/main/Docs)_

### Hello World

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=./Examples/HelloWorld/Basic.cpp) -->
<!-- The below code snippet is automatically added from ./Examples/Basic/HelloWorld.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std::string_literals;
using namespace sb::di;

int main()
{
    SevenBitRest rest;

    rest.mapGet("/", []() { return "Hello, world!"s; });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

Open link http://localhost:9090 to see results 

### Dependency Injection

Framework has build in dependency injection system, usage looks similar to asp net core

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=./Examples/ServiceProvider/Basic.cpp) -->
<!-- The below code snippet is automatically added from ./Examples/ServiceProvider/HelloWorld.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std;
using namespace sb::di;

struct Service
{
    string helloFromService() { return "Hello from service."; }
};

int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addScoped<Service>();

    rest.mapGet("/", [](Service &service) { return service.helloFromService(); });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

# Dependency Injection

## Service Scopes

Service can be registered as singleton, scoped or transient.

Singleton services are the same for every object and every request.

Scoped services are the same within a request, but different across different requests.

Transient services are always different, a new instance is provided to every service, due to that it must be injected using std::unique_ptr.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/Scopes.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/Scopes.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std;
using namespace string_literals;
using namespace sb::di;

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();

    rest.mapGet("/", [](SingletonService &singleton, ScopedService &scoped, unique_ptr<TransientService> transient) {
        return "Scoped Services."s;
    });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

This example shows how scope of service works by comparing pointers between different requests.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/ScopesExplanation.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/ScopesExplanation.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std;
using namespace std::string_literals;
using namespace sb::di;

std::string tabs(size_t indentation) { return string(indentation, '\t'); }

template <class T> string objectInfo(T *ptr, size_t indentation = 1)
{
    stringstream stream;
    stream << tabs(indentation) << typeid(T).name() << " pointer: " << ptr << std::endl;
    return stream.str();
}

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

class ConsumerAService
{
  private:
    SingletonService *_singleton;
    ScopedService *_scoped;
    unique_ptr<TransientService> _transient;

  public:
    ConsumerAService(SingletonService *singleton, ScopedService *scoped, unique_ptr<TransientService> transient)
    {
        _singleton = singleton;
        _scoped = scoped;
        _transient = std::move(transient);
    }

    string info(size_t indentation = 1)
    {
        return objectInfo(_singleton) + objectInfo(_scoped) + objectInfo(_transient.get());
    }
};

class ConsumerBService
{
  private:
    SingletonService *_singleton;
    ScopedService *_scoped;
    unique_ptr<TransientService> _transient;

  public:
    ConsumerBService(SingletonService *singleton, ScopedService *scoped, unique_ptr<TransientService> transient)
    {
        _singleton = singleton;
        _scoped = scoped;
        _transient = std::move(transient);
    }

    string info(size_t indentation = 1)
    {
        return objectInfo(_singleton) + objectInfo(_scoped) + objectInfo(_transient.get());
    }
};

int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();
    services.addScoped<ConsumerAService>();
    services.addScoped<ConsumerBService>();

    rest.mapGet("/", [](SingletonService &singleton, ScopedService &scoped, unique_ptr<TransientService> transient,
                        ConsumerAService &consumerA, ConsumerBService &consumerB) {
        auto fromEndpoint =
            "From endpoint:\n" + objectInfo(&singleton) + objectInfo(&scoped) + objectInfo(transient.get());
        auto fromConsumerA = "From consumer A:\n" + consumerA.info();
        auto fromConsumerB = "From consumer B:\n" + consumerB.info();

        return fromEndpoint + '\n' + fromConsumerA + '\n' + fromConsumerB + '\n';
    });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->
Possible output for first request:
```
From endpoint:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04130
	16TransientService pointer: 0x11ff05800

From consumer A:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04130
	16TransientService pointer: 0x11ff05810

From consumer B:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04130
	16TransientService pointer: 0x11ff058c0
```

Possible output for second request:
```
From endpoint:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04ac0
	16TransientService pointer: 0x11ff04340

From consumer A:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04ac0
	16TransientService pointer: 0x11ff04350

From consumer B:
	16SingletonService pointer: 0x11fe06ec0
	13ScopedService pointer: 0x11ff04ac0
	16TransientService pointer: 0x11ff04400
```

Singleton service address is always the same.
Scoped service address is same across request. Transient service address is always different.

## Injecting Interfaces

Services can be registered and injected by interfaces.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/Interfaces.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/Interfaces.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std;
using namespace sb::di;

struct IServiceA
{
    virtual std::string actionA() = 0;

    virtual ~IServiceA() = default;
};

struct IServiceB
{
    virtual string actionB() = 0;

    virtual ~IServiceB() = default;
};

class ServiceA final : public IServiceA
{
  public:
    string actionA() { return "actionA"; }
};

class ServiceB final : public IServiceB
{
  public:
    string actionB() { return "actionB"; }
};

class ConsumerService
{
  private:
    IServiceA *_serviceA;
    IServiceB *_serviceB;

  public:
    ConsumerService(IServiceA *serviceA, IServiceB *serviceB)
    {
        _serviceA = serviceA;
        _serviceB = serviceB;
    }

    string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addScoped<IServiceA, ServiceA>();
    services.addScoped<IServiceB, ServiceB>();
    services.addScoped<ConsumerService>();

    rest.mapGet("/", [](ConsumerService &consumer) { return consumer.execute(); });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

Multiple services can inherit one interface and can be injected using vector.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/InterfacesMultiple.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/InterfacesMultiple.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std;
using namespace sb::di;

struct IWorker
{
    virtual string work() = 0;

    virtual ~IWorker() = default;
};
class WorkerA final : public IWorker
{
  public:
    string work() { return "work A done!"; }
};
class WorkerB final : public IWorker
{
  public:
    string work() { return "work B done!"; }
};
class WorkerC final : public IWorker
{
  public:
    string work() { return "work C done!"; }
};

class ConsumerService
{
  private:
    vector<IWorker *> _workers;

  public:
    ConsumerService(vector<IWorker *> workers) { _workers = workers; }

    string workAll()
    {
        string result;
        for (auto worker : _workers)
        {
            result += worker->work() + " ";
        }
        return result;
    }
};
int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addScoped<IWorker, WorkerA>();
    services.addScoped<IWorker, WorkerB>();
    services.addScoped<IWorker, WorkerC>();
    services.addScoped<ConsumerService>();

    rest.mapGet("/", [](ConsumerService &consumer) { return consumer.workAll(); });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

## Rules

Dependency injection mechanism relies havily on template metaprogramming and it has some limitations. The most important one is that we cannot inject service into service using reference.

### General
* Define only one constructor in service
* If multiple services are registered by the same interface, all should have the same scope

### Injecting services
* Dont inject services by value: (T)
* Inject singleton and scoped services using one of:
  * Pointers: (T*)
  * Const pointner: (T* const)
  * Pointner to const object: (const T*)
  * Const pointner to const object: (const T* const)
* Inject transient service using std::unique_ptr: (unique_ptr<T>)
* Inject multiple services implementing specified interface using std::vector:
    * Transient (std::vector<std::unique_ptr<T>>)
    * Singleton/scoped (std::vector<T*>)

### Injecting services into endpoint callbacks
* All rules from "Injecting services" section are same here
* Singleton and scoped services can be injected additionally using:
  * Reference: (T&)
  * Const reference: (const T&)

Example:
<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/Rules.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/Rules.cpp -->
```cpp
#include "SevenBitRest.hpp"
#include <memory>

using namespace std;
using namespace string_literals;
using namespace sb::di;

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

class ConsumerService
{
  public:
    ConsumerService(const SingletonService *singleton, vector<ScopedService *> scoped,
                    vector<unique_ptr<TransientService>> trasients)
    {
    }
};
int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();
    services.addScoped<ConsumerService>();

    rest.mapGet("/", [](ConsumerService &consumer) { return "Injection rules."s; });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

## Using Service Provider

Service Provider object can be injected and can be used to manually get (scoped/singletons) or create (transient) services.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/SelfInjection.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/SelfInjection.cpp -->
```cpp
#include "SevenBitRest.hpp"

using namespace std::string_literals;
using namespace sb::di;

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();

    rest.mapGet("/", [](ServiceProvider &provider) {
        auto singleton = provider.getService<SingletonService>();
        auto scoped = provider.getService<ScopedService>();
        auto trasient = provider.createService<TransientService>();

        return "Service provider injection."s;
    });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->

## Service Factory Function

Factory function can be provided to manually create a service. Function should return unique_ptr and as an argument should take reference to service provider.

<!-- MARKDOWN-AUTO-DOCS:START (CODE:src=../Examples/ServiceProvider/FactoryFunction.cpp) -->
<!-- The below code snippet is automatically added from ../Examples/ServiceProvider/FactoryFunction.cpp -->
```cpp
#include "ServiceProvider.hpp"
#include "SevenBitRest.hpp"
#include <memory>

using namespace std;
using namespace sb::di;

class Service
{
  private:
    string _message;

  public:
    Service(std::string message) { _message = message; }

    string message() { return _message; }
};

int main()
{
    SevenBitRest rest;

    auto &services = rest.getServices();

    services.addScoped<Service>([](ServiceProvider &provider) { return make_unique<Service>("Hello from service!"); });

    rest.mapGet("/", [](Service &service) { return service.message(); });

    rest.run();
}
```
<!-- MARKDOWN-AUTO-DOCS:END -->



## Prebuild Binaries

<!-- MARKDOWN-AUTO-DOCS:START (WORKFLOW_ARTIFACT_TABLE) -->
<!-- MARKDOWN-AUTO-DOCS:END -->

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

## Contact

Project Link: [https://github.com/7bitcoder/SevenBitRest](https://github.com/7bitcoder/SevenBitRest)

@7bitcoder 2022
