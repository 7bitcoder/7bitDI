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

template <class TService, bool create> auto GetOrCreate(IServiceProvider &provider)
{
    if constexpr (create)
    {
        return provider.createService<TService>();
    }
    else
    {
        return &provider.getService<TService>();
    }
}

template <class TService, bool create>
void compareServices(IServiceProvider &rootProvider, IServiceProvider &scopedProvider)
{
    std::cout << "rootProvider \t == rootProvider:\t"
              << (GetOrCreate<TService, create>(rootProvider) == GetOrCreate<TService, create>(rootProvider))
              << std::endl;
    std::cout << "rootProvider \t == scopedProvider:\t"
              << (GetOrCreate<TService, create>(rootProvider) == GetOrCreate<TService, create>(scopedProvider))
              << std::endl;
    std::cout << "scopedProvider \t == scopedProvider:\t"
              << (GetOrCreate<TService, create>(scopedProvider) == GetOrCreate<TService, create>(scopedProvider))
              << std::endl;
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

    std::cout << std::endl << "Singletons comparision" << std::endl;
    compareServices<SingletonService, false>(*rootProvider, *scopedProvider);

    std::cout << std::endl << "Scoped comparision" << std::endl;
    compareServices<ScopedService, false>(*rootProvider, *scopedProvider);

    std::cout << std::endl << "Transient comparision" << std::endl;
    compareServices<TransientService, true>(*rootProvider, *scopedProvider);
    return 0;
}