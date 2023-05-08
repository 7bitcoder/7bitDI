#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

using namespace std;
using namespace sb::di;

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

    IServiceProvider::Ptr provider = ServiceCollection{}
            .addSingleton<Service>([](IServiceProvider& provider) { return make_unique<Service>("Hello from service!"); })
            .buildServiceProvider();

    Service &service = provider->getService<Service>();

    cout << service.message();

    return 0;
}
