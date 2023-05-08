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

class ConsumerService
{
  public:
    ConsumerService(const SingletonService *singleton, vector<ScopedService *> scoped,
                    vector<unique_ptr<TransientService>> trasients)
    {
    }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<SingletonService>()
                                         .addScoped<ScopedService>()
                                         .addTransient<TransientService>()
                                         .addScoped<ConsumerService>()
                                         .buildServiceProvider();

    ConsumerService &consumer = provider->getService<ConsumerService>();

    return 0;
}