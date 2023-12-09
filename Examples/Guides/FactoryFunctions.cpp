#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>

using namespace sb::di;

class ServiceA
{
  private:
    std::string _message;

  public:
    ServiceA(std::string message) { _message = message; }

    std::string message() { return _message; }
};

class ServiceB
{
  private:
    ServiceA &_serviceA;

  public:
    ServiceB(ServiceA *serviceA) : _serviceA(*serviceA) {}

    std::string message() { return _serviceA.message(); }
};

int main()
{
    ServiceProvider::Ptr provider =
        ServiceCollection{}
            .addSingleton<ServiceA>([]() { return std::make_unique<ServiceA>("Hello from service!"); })
            .addSingleton<ServiceB>([](ServiceA *serviceA) { return std::make_unique<ServiceB>(serviceA); })
            .buildServiceProvider();

    ServiceA &serviceA = provider->getService<ServiceA>();
    ServiceB &serviceB = provider->getService<ServiceB>();

    std::cout << serviceB.message();

    return 0;
}
