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

  public:
    explicit ServiceExecutor(ServiceProvider &provider)
        : _serviceA(provider.getKeyedService<Service>("serviceA")),
          _serviceB(provider.getKeyedService<Service>("serviceB"))
    {
        assert(&_serviceA != &_serviceB);
        assert(!provider.tryGetService<Service>());
    }

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.action() + ", " + _serviceB.action() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addKeyedSingleton<Service>("serviceA")
                                   .addKeyedSingleton<Service>("serviceB")
                                   .addScoped<IServiceExecutor, ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
