#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

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

template <class TService, bool Get> auto getOrCreate(IServiceProvider &provider)
{
    if constexpr (Get)
    {
        return &provider.getService<TService>();
    }
    else
    {
        return provider.createService<TService>();
    }
}

template <class TService, bool Get> void compareServices(IServiceProvider &root, IServiceProvider &scoped)
{
    std::cout << "rootProvider \t == rootProvider:\t"
              << (getOrCreate<TService, Get>(root) == getOrCreate<TService, Get>(root)) << std::endl;
    std::cout << "rootProvider \t == scopedProvider:\t"
              << (getOrCreate<TService, Get>(root) == getOrCreate<TService, Get>(scoped)) << std::endl;
    std::cout << "scopedProvider \t == scopedProvider:\t"
              << (getOrCreate<TService, Get>(scoped) == getOrCreate<TService, Get>(scoped)) << std::endl;
}

int main()
{
    IServiceProvider::Ptr rootProvider = ServiceCollection{}
                                             .addSingleton<SingletonService>()
                                             .addScoped<ScopedService>()
                                             .addTransient<TransientService>()
                                             .buildServiceProvider();

    // Accessing Services
    SingletonService &singleton = rootProvider->getService<SingletonService>();
    ScopedService &scoped = rootProvider->getService<ScopedService>();
    std::unique_ptr<TransientService> transient = rootProvider->createService<TransientService>();

    IServiceProvider::Ptr scopedProvider = rootProvider->createScope();

    std::cout << std::endl << "Singletons comparison" << std::endl;
    compareServices<SingletonService, true>(*rootProvider, *scopedProvider);

    std::cout << std::endl << "Scoped comparison" << std::endl;
    compareServices<ScopedService, true>(*rootProvider, *scopedProvider);

    std::cout << std::endl << "Transient comparison" << std::endl;
    compareServices<TransientService, false>(*rootProvider, *scopedProvider);
    return 0;
}