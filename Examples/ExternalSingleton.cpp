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

    Service externalSingleton{"Hello from service!"};
    IServiceProvider::Ptr provider = ServiceCollection{}.addSingleton(&externalSingleton).buildServiceProvider();

    Service &service = provider->getService<Service>();

    cout << service.message();

    return 0;
}
