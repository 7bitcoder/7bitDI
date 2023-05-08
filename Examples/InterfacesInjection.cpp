#include <SevenBit/DI.hpp>
#include <iostream>

using namespace std;
using namespace sb::di;

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
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ConsumerService(IServiceA *serviceA, std::unique_ptr<IServiceB> serviceB)
    {
        _serviceA = serviceA;
        _serviceB = std::move(serviceB);
    }

    string execute() { return _serviceA->actionA() + ", " + _serviceB->actionB() + " executed."; }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<IServiceA, ServiceA>()
                                         .addTransient<IServiceB, ServiceB>()
                                         .addScoped<ConsumerService>()
                                         .buildServiceProvider();

    ConsumerService &consumer = provider->getService<ConsumerService>();

    cout << consumer.execute();

    return 0;
}