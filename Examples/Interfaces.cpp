#include "SevenBitRest.hpp"

using namespace std;
using namespace sb;

struct IServiceA
{
    virtual std::string actionA() = 0;

    virtual ~IServiceA() = default;
};

struct IServiceB
{
    virtual string actionB() = 0;

    virtual ~IServiceB() = default;
};

class ServiceA final : public IServiceA
{
  public:
    string actionA() { return "actionA"; }
};

class ServiceB final : public IServiceB
{
  public:
    string actionB() { return "actionB"; }
};

class ConsumerService
{
  private:
    IServiceA *_serviceA;
    IServiceB *_serviceB;

  public:
    ConsumerService(IServiceA *serviceA, IServiceB *serviceB)
    {
        _serviceA = serviceA;
        _serviceB = serviceB;
    }

    string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addScoped<IServiceA, ServiceA>();
    services.addScoped<IServiceB, ServiceB>();
    services.addScoped<ConsumerService>();

    auto rest = builder.build();

    rest.mapGet("/", [](ConsumerService &consumer) { return consumer.execute(); });

    rest.run();
}