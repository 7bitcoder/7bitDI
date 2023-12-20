#include <SevenBit/DI.hpp>
#include <cassert>
#include <memory>

using namespace sb::di;

struct NotReqisteredService
{
};
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
    ServiceExecutor(NotReqisteredService *notRegistered, const SingletonService &singleton,
                    const std::vector<ScopedService *> scoped, std::vector<std::unique_ptr<TransientService>> trasients)
    {
        assert(notRegistered == nullptr);
        assert(scoped.size() == 1);
        assert(trasients.size() == 1);
    }
};
int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<SingletonService>()
                                   .addScoped<ScopedService>()
                                   .addTransient<TransientService>()
                                   .addScoped<ServiceExecutor>()
                                   .buildServiceProvider();

    auto &consumer = provider.getService<ServiceExecutor>();

    return 0;
}
