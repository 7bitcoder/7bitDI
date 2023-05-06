#include "SevenBitRest.hpp"

using namespace std;
using namespace string_literals;
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
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();

    auto rest = builder.build();

    rest.mapGet("/", [](SingletonService &singleton, ScopedService &scoped, unique_ptr<TransientService> transient) {
        return "Scoped Services."s;
    });

    rest.run();
}