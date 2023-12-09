#include <SevenBit/DI.hpp>

using namespace sb::di;

struct ServiceA
{
};
struct ServiceB
{
};
struct ServiceC
{
};
int main()
{
    // Service registration
    ServiceCollection services;
    services.addSingleton<ServiceA>();
    services.addSingleton<ServiceB>();
    services.addSingleton<ServiceC>();

    const ServiceProvider::Ptr provider = services.buildServiceProvider();

    // Accessing services
    auto &serviceA = provider->getService<ServiceA>();
    auto &serviceB = provider->getService<ServiceB>();
    auto &serviceC = provider->getService<ServiceC>();

    return 0;
}
