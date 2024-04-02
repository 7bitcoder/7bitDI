#include <SevenBit/DI.hpp>
#include <SevenBit/DI/Utils/Register.hpp>
#include <iostream>

using namespace sb::di;

struct IServiceA
{
    virtual std::string actionA() = 0;

    virtual ~IServiceA() = default;
};

struct IServiceB
{
    virtual std::string actionB() = 0;

    virtual ~IServiceB() = default;
};

struct IServiceExecutor
{
    [[nodiscard]] virtual std::string execute() const = 0;

    virtual ~IServiceExecutor() = default;
};

struct ServiceA final : IServiceA, RegisterSingleton<IServiceA, ServiceA>
{
    std::string actionA() override { return "actionA"; }
};

struct ServiceB final : IServiceB, RegisterTransient<IServiceB, ServiceB>
{
    std::string actionB() override { return "actionB"; }
};

class ServiceExecutor final : public IServiceExecutor, public RegisterScoped<IServiceExecutor, ServiceExecutor>
{
    IServiceA &_serviceA;
    std::unique_ptr<IServiceB> _serviceB;

  public:
    ServiceExecutor(IServiceA &serviceA, std::unique_ptr<IServiceB> serviceB)
        : _serviceA(serviceA), _serviceB(std::move(serviceB))
    {
    }

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = GlobalServices::instance().buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
