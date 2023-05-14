#include <SevenBit/DI.hpp>
#include <iostream>

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

    IServiceProvider::Ptr provider = services.buildServiceProvider();

    // Accessing services
    ServiceA &serviceA = provider->getService<ServiceA>();
    ServiceB &serviceB = provider->getService<ServiceB>();
    ServiceC &serviceC = provider->getService<ServiceC>();

    return 0;
}