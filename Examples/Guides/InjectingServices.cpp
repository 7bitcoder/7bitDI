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
