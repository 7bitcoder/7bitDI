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

struct IServiceExecutor
{
    [[nodiscard]] virtual std::string execute() const = 0;

    virtual ~IServiceExecutor() = default;
};

struct ServiceA final : IServiceA
{
    std::string actionA() override { return "actionA"; }
};

struct ServiceB final : IServiceB
{
    std::string actionB() override { return "actionB"; }
};

class ServiceExecutor final : public IServiceExecutor
{
    IServiceA &_serviceA;
    std::unique_ptr<ServiceB> _serviceB;

  public:
    ServiceExecutor(IServiceA &serviceA, std::unique_ptr<ServiceB> serviceB)
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

    ServiceProvider provider =
        ServiceCollection{}
            .addSingleton<IServiceA, ServiceA>()
            .addTransient<IServiceB, ServiceB>()
            .addSingleton<ServiceA>() // can be added one more time due to checkServiceGlobalUniqueness = false
            .addTransient<ServiceB>() // can be added one more time due to checkServiceGlobalUniqueness = false
            .addScoped<IServiceExecutor, ServiceExecutor>()
            .buildServiceProvider(options);

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
