#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

using namespace std;
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
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<SingletonService>()
                                         .addScoped<ScopedService>()
                                         .addTransient<TransientService>()
                                         .buildServiceProvider();

    SingletonService &singleton = provider->getService<SingletonService>();
    ScopedService &scoped = provider->getService<ScopedService>();
    std::unique_ptr<TransientService> transient = provider->createService<TransientService>();

    return 0;
}