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

struct ServiceA : IServiceA
{
    std::string actionA() override { return "actionA"; }
};

struct ServiceB : IServiceB
{
    std::string actionB() override { return "actionB"; }
};

struct Service final : ServiceA, ServiceB
{
    std::string actionA() override { return "actionA from top service"; }
    std::string actionB() override { return "actionB from top service"; }
};

class ServiceExecutor
{
    IServiceA &_serviceA;
    IServiceB &_serviceB;

  public:
    ServiceExecutor(IServiceA &serviceA, IServiceB &serviceB) : _serviceA(serviceA), _serviceB(serviceB) {}

    [[nodiscard]] std::string execute() const
    {
        return _serviceA.actionA() + ", " + _serviceB.actionB() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<Service>()
                                   .addAlias<ServiceA, Service>()
                                   .addAlias<ServiceB, Service>()
                                   .addAlias<IServiceA, ServiceA>()
                                   .addAlias<IServiceB, ServiceB>()
                                   .addScoped<ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<ServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
