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

class ServiceExecutor
{
    ServiceA &_serviceA;
    std::unique_ptr<ServiceB> _serviceB;

  public:
    explicit ServiceExecutor(ServiceProvider &provider)
        : _serviceA(provider.getService<ServiceA>()), _serviceB(provider.createService<ServiceB>())
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
                                   .addSingleton<ServiceA>()
                                   .addTransient<ServiceB>()
                                   .addScoped<ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &consumer = provider.getService<ServiceExecutor>();

    std::cout << consumer.execute();
    return 0;
}
