#include "ServiceProvider.hpp"
#include "SevenBitRest.hpp"
#include <memory>

using namespace std;
using namespace sb;

class Service
{
  private:
    string _message;

  public:
    Service(std::string message) { _message = message; }

    string message() { return _message; }
};

int main()
{
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addScoped<Service>([](ServiceProvider &provider) { return make_unique<Service>("Hello from service!"); });

    auto rest = builder.build();

    rest.mapGet("/", [](Service &service) { return service.message(); });

    rest.run();
}