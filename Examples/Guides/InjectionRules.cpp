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

class ServiceExecutor
{
  public:
    ServiceExecutor(const SingletonService *singleton, std::vector<ScopedService *> scoped,
                    std::vector<std::unique_ptr<TransientService>> trasients)
    {
    }
};
int main()
{
    ServiceProvider::Ptr provider = ServiceCollection{}
                                        .addSingleton<SingletonService>()
                                        .addScoped<ScopedService>()
                                        .addTransient<TransientService>()
                                        .addScoped<ServiceExecutor>()
                                        .buildServiceProvider();

    ServiceExecutor &consumer = provider->getService<ServiceExecutor>();

    return 0;
}
