#include <SevenBit/DI.hpp>
#include <cassert>
#include <iostream>

using namespace sb::di;

struct Service
{
    std::string action() { return "action"; }
};

struct IServiceExecutor
{
    [[nodiscard]] virtual std::string execute() const = 0;

    virtual ~IServiceExecutor() = default;
};

class ServiceExecutor final : public IServiceExecutor
{
    Service &_serviceA;
    Service &_serviceB;
    Service &_serviceC;

  public:
    explicit ServiceExecutor(ServiceProvider &provider)
        : _serviceA(provider.getService<Service>()), _serviceB(provider.getKeyedService<Service>("serviceB")),
          _serviceC(provider.getKeyedService<Service>("serviceC"))
    {
        assert(&_serviceA != &_serviceB);
        assert(&_serviceB != &_serviceC);
        assert(&_serviceA != &_serviceC);
    }

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.action() + ", " + _serviceB.action() + ", " + _serviceC.action() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<Service>()
                                   .addKeyedSingleton<Service>("serviceB")
                                   .addKeyedSingleton<Service>("serviceC")
                                   .addScoped<IServiceExecutor, ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
