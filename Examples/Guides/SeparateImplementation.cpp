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
    std::string helloFromService() override { return "Hello from service."; }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}.addSingleton<IService, Service>().buildServiceProvider();

    auto &service = provider.getService<IService>();

    std::cout << service.helloFromService();

    return 0;
}
