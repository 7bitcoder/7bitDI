#include <SevenBit/DI.hpp>
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

struct ServiceA final : IServiceA
{
    std::string actionA() override { return "actionA"; }
};

struct ServiceB final : IServiceB
{
    std::string actionB() override { return "actionB"; }
};

class ServiceExecutor final : public Injected<IServiceExecutor>
{
    IServiceA &_serviceA = inject<IServiceA>();
    IServiceA *_optionalServiceA = tryInject<IServiceA>();
    std::vector<IServiceA *> _allServiceA = injectAll<IServiceA>();

    std::unique_ptr<IServiceB> _serviceB = make<IServiceB>();
    std::unique_ptr<IServiceB> _optionalServiceB = tryMake<IServiceB>();
    std::vector<std::unique_ptr<IServiceB>> _allServiceB = makeAll<IServiceB>();

  public:
    using Injected::Injected;

    [[nodiscard]] std::string execute() const override
    {
        return _serviceA.actionA() + ", " + _serviceB->actionB() + " executed.";
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IServiceA, ServiceA>()
                                   .addTransient<IServiceB, ServiceB>()
                                   .addScoped<IServiceExecutor, ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &executor = provider.getService<IServiceExecutor>();

    std::cout << executor.execute();
    return 0;
}
