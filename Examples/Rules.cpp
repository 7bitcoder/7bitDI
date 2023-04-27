#include "SevenBitRest.hpp"
#include <memory>

using namespace std;
using namespace string_literals;
using namespace sb;

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
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();
    services.addScoped<ConsumerService>();

    auto rest = builder.build();

    rest.mapGet("/", [](ConsumerService &consumer) { return "Injection rules."s; });

    rest.run();
}