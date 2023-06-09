#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

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
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<Service>([](IServiceProvider &provider) {
                                             return std::make_unique<Service>("Hello from service!");
                                         })
                                         .buildServiceProvider();

    Service &service = provider->getService<Service>();

    std::cout << service.message();

    return 0;
}
