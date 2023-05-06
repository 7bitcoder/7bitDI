#include "SevenBitRest.hpp"

using namespace std::string_literals;
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

    rest.mapGet("/", [](ServiceProvider &provider) {
        auto singleton = provider.getService<SingletonService>();
        auto scoped = provider.getService<ScopedService>();
        auto trasient = provider.createService<TransientService>();

        return "Service provider injection."s;
    });

    rest.run();
}