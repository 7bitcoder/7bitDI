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

