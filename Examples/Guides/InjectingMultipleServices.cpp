#include <SevenBit/DI.hpp>
#include <iostream>
#include <utility>

using namespace sb::di;

struct IWorker
{
    virtual std::string work() = 0;

    virtual ~IWorker() = default;
};
struct WorkerA final : IWorker
{
    std::string work() override { return "work A done!"; }
};
struct WorkerB final : IWorker
{
    std::string work() override { return "work B done!"; }
};
struct WorkerC final : IWorker
{
    std::string work() override { return "work C done!"; }
};

class ServiceExecutor
{
    std::vector<IWorker *> _workers;

  public:
    explicit ServiceExecutor(std::vector<IWorker *> workers) : _workers(std::move(workers)) {}

    [[nodiscard]] std::string workAll() const
    {
        std::string result;
        for (const auto worker : _workers)
        {
            result += worker->work() + " ";
        }
        return result;
    }
};
int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IWorker, WorkerA>()
                                   .addSingleton<IWorker, WorkerB>()
                                   .addSingleton<IWorker, WorkerC>()
                                   .addSingleton<ServiceExecutor>()
                                   .buildServiceProvider();

    const auto &consumer = provider.getService<ServiceExecutor>();

    std::cout << "work all: " << consumer.workAll() << std::endl;
    std::cout << "single work: " << provider.getService<IWorker>().work();

    return 0;
}
