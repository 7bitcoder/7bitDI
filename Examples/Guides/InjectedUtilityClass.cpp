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

class ServiceExecutor final : public Injected<IServiceExecutor>
{
    ServiceA &_serviceA = inject<ServiceA>();
    ServiceA *_optionalServiceA = tryInject<ServiceA>();
    std::vector<ServiceA *> _allServiceA = injectAll<ServiceA>();

    std::unique_ptr<ServiceB> _serviceB = make<ServiceB>();
    std::unique_ptr<ServiceB> _optionalServiceB = tryMake<ServiceB>();
    std::vector<std::unique_ptr<ServiceB>> _allServiceB = makeAll<ServiceB>();

  public:
    using Injected::Injected;

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<ServiceA>()
                                   .addTransient<ServiceB>()
                                   .addScoped<IServiceExecutor, ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
