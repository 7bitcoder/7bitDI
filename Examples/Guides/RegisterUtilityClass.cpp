#include <SevenBit/DI.hpp>
#include <SevenBit/DI/Utils/Injected.hpp>
#include <iostream>

using namespace sb::di;

struct ServiceA : RegisterSingleton<ServiceA>
{
    std::string actionA() { return "actionA"; }
};

struct ServiceB : RegisterTransient<ServiceB>
{
    std::string actionB() { return "actionB"; }
};

struct IServiceExecutor
{
    [[nodiscard]] virtual std::string execute() const = 0;

    virtual ~IServiceExecutor() = default;
};

class ServiceExecutor final : public InjectedSingleton<IServiceExecutor, ServiceExecutor>
{
    ServiceA &_serviceA = inject();
    ServiceA *_optionalServiceA = inject();
    // std::vector<ServiceA *> _allServiceA = inject();

    std::unique_ptr<ServiceB> _serviceB = inject();
    std::vector<std::unique_ptr<ServiceB>> _allServiceB = inject();

  public:
    using InjectedSingleton::InjectedSingleton;

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = GlobalServices::instance().buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
