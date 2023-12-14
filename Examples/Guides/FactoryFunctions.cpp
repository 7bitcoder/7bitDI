#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>
#include <utility>

using namespace sb::di;

class ServiceA
{
    std::string _message;

  public:
    explicit ServiceA(std::string message) { _message = std::move(message); }

    std::string message() { return _message; }
};

class ServiceB
{
    ServiceA &_serviceA;

  public:
    explicit ServiceB(ServiceA *serviceA) : _serviceA(*serviceA) {}

    [[nodiscard]] std::string message() const { return _serviceA.message(); }
};

int main()
{
    ServiceProvider provider =
        ServiceCollection{}
            .addSingleton<ServiceA>([] { return std::make_unique<ServiceA>("Hello from service!"); })
            .addSingleton<ServiceB>([](ServiceA *serviceA) { return std::make_unique<ServiceB>(serviceA); })
            .buildServiceProvider();

    auto &serviceA = provider.getService<ServiceA>();
    const auto &serviceB = provider.getService<ServiceB>();

    std::cout << serviceB.message();

    return 0;
}
