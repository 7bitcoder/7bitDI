#include <SevenBit/DI.hpp>
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
    ServiceExecutor(const SingletonService *singleton, const std::vector<ScopedService *> &scoped,
                    std::vector<std::unique_ptr<TransientService>> trasients)
    {
    }
};
int main()
{
    const ServiceProvider::Ptr provider = ServiceCollection{}
                                              .addSingleton<SingletonService>()
                                              .addScoped<ScopedService>()
                                              .addTransient<TransientService>()
                                              .addScoped<ServiceExecutor>()
                                              .buildServiceProvider();

    auto &consumer = provider->getService<ServiceExecutor>();

    return 0;
}
