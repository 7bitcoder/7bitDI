#include <SevenBit/DI.hpp>
#include <cassert>
#include <iostream>

using namespace sb::di;

struct IService
{
    virtual std::string action() = 0;

    virtual ~IService() = default;
};

struct Service final : IService
{
    std::string action() override { return "action"; }
};

class ServiceExecutor
{
    IService &_service;
    Service &_implService;

  public:
    explicit ServiceExecutor(IService &service, Service &implService) : _service(service), _implService(implService)
    {
        assert(&service != &implService);
    }

    [[nodiscard]] std::string execute() const
    {
        return _service.action() + ", " + _implService.action() + " executed.";
    }
};

int main()
{
    ServiceProviderOptions options;
    options.strongDestructionOrder = true;
    options.prebuildSingletons = true;
    options.checkServiceGlobalUniqueness = false;

    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IService, Service>()
                                   .addSingleton<Service>() // can be added one more time as separate service
                                                            // due to checkServiceGlobalUniqueness = false
                                   .addScoped<ServiceExecutor>()
                                   .buildServiceProvider(options);

    const auto &executor = provider.getService<ServiceExecutor>();
    std::cout << executor.execute();
    return 0;
}
