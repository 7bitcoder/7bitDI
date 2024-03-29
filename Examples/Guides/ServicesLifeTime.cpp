#include <SevenBit/DI.hpp>
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

template <class TService, bool CreateService>
bool compareServicePtrs(ServiceProvider &provider1, ServiceProvider &provider2)
{
    return CreateService ? provider1.createService<TService>() == provider2.createService<TService>()
                         : &provider1.getService<TService>() == &provider2.getService<TService>();
}

template <class TService, bool CreateService = false>
void compareServices(ServiceProvider &root, ServiceProvider &scoped)
{
    std::cout << "rootProvider \t == rootProvider:\t";
    std::cout << compareServicePtrs<TService, CreateService>(root, root) << std::endl;
    std::cout << "rootProvider \t == scopedProvider:\t";
    std::cout << compareServicePtrs<TService, CreateService>(root, scoped) << std::endl;
    std::cout << "scopedProvider \t == scopedProvider:\t";
    std::cout << compareServicePtrs<TService, CreateService>(scoped, scoped) << std::endl;
}

int main()
{
    ServiceProvider rootProvider = ServiceCollection{}
                                       .addSingleton<SingletonService>()
                                       .addScoped<ScopedService>()
                                       .addTransient<TransientService>()
                                       .buildServiceProvider();

    // Accessing Services
    SingletonService &singleton = rootProvider.getService<SingletonService>();
    ScopedService &scoped = rootProvider.getService<ScopedService>();
    std::unique_ptr<TransientService> transient = rootProvider.createService<TransientService>();

    ServiceProvider scopedProvider = rootProvider.createScope();

    std::cout << std::endl << "Singletons comparison" << std::endl;
    compareServices<SingletonService>(rootProvider, scopedProvider);

    std::cout << std::endl << "Scoped comparison" << std::endl;
    compareServices<ScopedService>(rootProvider, scopedProvider);

    std::cout << std::endl << "Transient comparison" << std::endl;
    compareServices<TransientService, true>(rootProvider, scopedProvider);
    return 0;
}
