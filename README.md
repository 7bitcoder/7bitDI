[![CI](https://github.com/7bitcoder/7bitinjector/actions/workflows/CI.yml/badge.svg?branch=master)](https://github.com/7bitcoder/7bitinjector/actions/workflows/CI.ym)
<div align="center">

  <img src="Docs/_static/7bitInjector-logo.svg" alt="logo" width="500" height="auto" />

  <h4>
    C++17 simple dependency injection library!
  </h4>

  <p>
    7bitInjector is a simple C++ dependency injection library, designed to be as easy to use as possible, the main inspiration was asp net core dependency injection system. 
  </p>
   
<h4>
    <a href="#">Documentation & Tutorials</a>
</div>

<br />

### Built With

* [Google Test](https://github.com/google/googletest)
  
### Simple Usage


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

class ServiceA final : public IServiceA
{
  public:
    std::string actionA() { return "actionA"; }
};

class ServiceB final : public IServiceB
{
  public:
    std::string actionB() { return "actionB"; }
};

class ConsumerService
{
  private:
    IServiceA *_serviceA;
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ConsumerService(IServiceA *serviceA, std::unique_ptr<IServiceB> serviceB)
    {
        _serviceA = serviceA;
        _serviceB = std::move(serviceB);
    }

    std::string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<IServiceA, ServiceA>()
                                         .addTransient<IServiceB, ServiceB>()
                                         .addScoped<ConsumerService>()
                                         .buildServiceProvider();

    ConsumerService &consumer = provider->getService<ConsumerService>();

    std::cout << consumer.execute();

    return 0;
}
```
Output

```console
actionA, actionB executed.
```

More examples and tutorials available on  [documentation](#) page

@7bitcoder Sylwester Dawida 2023

