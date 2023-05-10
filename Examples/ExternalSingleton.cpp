#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

class Service
{
  private:
    std::string _message;

  public:
    Service(std::string message) { _message = message; }

    std::string message() { return _message; }
};

int main()
{
    Service externalSingleton{"Hello from service!"};
    IServiceProvider::Ptr provider = ServiceCollection{}.addSingleton(&externalSingleton).buildServiceProvider();

    Service &service = provider->getService<Service>();

    std::cout << service.message();

    return 0;
}
