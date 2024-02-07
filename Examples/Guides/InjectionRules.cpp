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
    ServiceExecutor(NotReqisteredService *notRegistered, SingletonService &singleton, ScopedService *scoped,
                    std::unique_ptr<TransientService> transient, TransientService transientInPlace,
                    std::vector<ScopedService *> scopedList,
                    std::vector<std::unique_ptr<TransientService>> trasientList)
    {
        assert(notRegistered == nullptr);
        assert(scoped != nullptr);
        assert(transient != nullptr);
        assert(scopedList.size() == 1);
        assert(trasientList.size() == 1);
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
