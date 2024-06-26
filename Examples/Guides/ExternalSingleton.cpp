#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

class Service
{
    std::string _message;

  public:
    explicit Service(std::string message) { _message = std::move(message); }

    std::string message() { return _message; }
};

int main()
{
    Service externalSingleton{"Hello from service!"};
    ServiceProvider provider = ServiceCollection{}.addSingleton(&externalSingleton).buildServiceProvider();

    auto &service = provider.getService<Service>();

    std::cout << service.message();
    return 0;
}
