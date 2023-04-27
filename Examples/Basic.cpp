#include "SevenBitRest.hpp"

using namespace std;
using namespace sb;

struct Service
{
    string helloFromService() { return "Hello from service."; }
};

int main()
{
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addScoped<Service>();

    auto rest = builder.build();

    rest.mapGet("/", [](Service &service) { return service.helloFromService(); });

    rest.run();
}