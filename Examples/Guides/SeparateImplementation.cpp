#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

struct IService
{
    virtual std::string helloFromService() = 0;

    virtual ~IService() = default;
};

class Service final : public IService
{
  public:
    std::string helloFromService() { return "Hello from instanceValidity."; }
};

int main()
{
    ServiceProvider::Ptr provider = ServiceCollection{}.addSingleton<IService, Service>().buildServiceProvider();

    IService &service = provider->getService<IService>();

    std::cout << service.helloFromService();

    return 0;
}
