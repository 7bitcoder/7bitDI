#include <SevenBit/DI.hpp>
#include <iostream>

using namespace std;
using namespace sb::di;

struct IService
{
    virtual std::string helloFromService() = 0;

    virtual ~IService() = default;
};

class Service final : public IService
{
  public:
    string helloFromService() { return "Hello from service."; }
};

int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}.addSingleton<IService, Service>().buildServiceProvider();

    IService &service = provider->getService<IService>();

    cout << service.helloFromService();

    return 0;
}