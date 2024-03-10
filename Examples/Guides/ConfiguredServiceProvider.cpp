#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

struct ServiceA
{
    std::string actionA() { return "actionA"; }
};

struct ServiceB
{
    std::string actionB() { return "actionB"; }
};

struct IServiceExecutor
{
    [[nodiscard]] virtual std::string execute() const = 0;

    virtual ~IServiceExecutor() = default;
};

class ServiceExecutor final : public IServiceExecutor
{
    ServiceA &_serviceA;
    std::unique_ptr<ServiceB> _serviceB;

  public:
    ServiceExecutor(ServiceA &serviceA, std::unique_ptr<ServiceB> serviceB)
        : _serviceA(serviceA), _serviceB(std::move(serviceB))
    {
    }

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};

int main()
{
    ServiceProviderOptions options;
    options.strongDestructionOrder = true;
    options.prebuildSingletons = true;
    options.checkServiceGlobalUniqueness = false;

    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<ServiceA>()
                                   .addTransient<ServiceB>()
                                   .addScoped<IServiceExecutor, ServiceExecutor>()
                                   .addScoped<ServiceExecutor>() // can be added one more time as separate service due
                                                                 // to checkServiceGlobalUniqueness = false
                                   .buildServiceProvider(options);

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
