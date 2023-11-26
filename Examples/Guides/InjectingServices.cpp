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

struct ServiceA final : public IServiceA
{
    std::string actionA() { return "actionA"; }
};

struct ServiceB final : public IServiceB
{
    std::string actionB() { return "actionB"; }
};

class ServiceExecutor
{
    IServiceA *_serviceA;
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ServiceExecutor(IServiceA *serviceA, std::unique_ptr<IServiceB> serviceB)
    {
        _serviceA = serviceA;
        _serviceB = std::move(serviceB);
    }

    std::string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    ServiceProvider::Ptr provider = ServiceCollection{}
                                        .addSingleton<IServiceA, ServiceA>()
                                        .addTransient<IServiceB, ServiceB>()
                                        .addScoped<ServiceExecutor>()
                                        .buildServiceProvider();

    ServiceExecutor &executor = provider->getService<ServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
