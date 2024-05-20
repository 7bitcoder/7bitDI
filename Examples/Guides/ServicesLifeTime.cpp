#include <SevenBit/DI.hpp>
#include <cassert>
#include <iostream>

using namespace sb::di;

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

int main()
{
    ServiceProvider rootProvider = ServiceCollection{}
                                       .addSingleton<SingletonService>()
                                       .addScoped<ScopedService>()
                                       .addTransient<TransientService>()
                                       .buildServiceProvider();

    // Accessing services
    SingletonService &rootSingleton = rootProvider.getService<SingletonService>();
    ScopedService &rootScoped = rootProvider.getService<ScopedService>();
    std::unique_ptr<TransientService> rootTransient = rootProvider.createService<TransientService>();

    ServiceProvider scopedProvider = rootProvider.createScope();

    // Accessing scoped services
    SingletonService &singleton = scopedProvider.getService<SingletonService>();
    ScopedService &scoped = scopedProvider.getService<ScopedService>();
    std::unique_ptr<TransientService> transient = scopedProvider.createService<TransientService>();

    assert(&rootSingleton == &singleton); // The same service for root and scoped provider
    assert(&rootScoped != &scoped);       // Different service for root and scoped provider
    assert(rootTransient != transient);   // Always different service (trivially different unique ptrs)

    std::cout << "Service Addresses Table" << std::endl;
    std::cout << "\t\t\trootProvider\tscopedProvider" << std::endl;
    std::cout << "singleton\t" << &rootSingleton << "\t" << &singleton << std::endl;
    std::cout << "scoped\t\t" << &rootScoped << "\t" << &scoped << std::endl;
    std::cout << "transient\t" << rootTransient.get() << "\t" << transient.get() << std::endl;
    return 0;
}
