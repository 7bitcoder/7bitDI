#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

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

class ServiceExecutor
{
  private:
    IServiceA *_serviceA;
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ServiceExecutor(IServiceProvider *provider)
    {
        _serviceA = &provider->getService<IServiceA>();
        _serviceB = provider->createService<IServiceB>();
    }

    std::string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<IServiceA, ServiceA>()
                                         .addTransient<IServiceB, ServiceB>()
                                         .addScoped<ServiceExecutor>()
                                         .buildServiceProvider();

    ServiceExecutor &consumer = provider->getService<ServiceExecutor>();

    std::cout << consumer.execute();

    return 0;
}