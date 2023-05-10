#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

struct IWorker
{
    virtual std::string work() = 0;

    virtual ~IWorker() = default;
};
class WorkerA final : public IWorker
{
  public:
    std::string work() { return "work A done!"; }
};
class WorkerB final : public IWorker
{
  public:
    std::string work() { return "work B done!"; }
};
class WorkerC final : public IWorker
{
  public:
    std::string work() { return "work C done!"; }
};

class ServiceExecutor
{
  private:
    std::vector<IWorker *> _workers;

  public:
    ServiceExecutor(std::vector<IWorker *> workers) { _workers = workers; }

    std::string workAll()
    {
        std::string result;
        for (auto worker : _workers)
        {
            result += worker->work() + " ";
        }
        return result;
    }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<IWorker, WorkerA>()
                                         .addSingleton<IWorker, WorkerB>()
                                         .addSingleton<IWorker, WorkerC>()
                                         .addSingleton<ServiceExecutor>()
                                         .buildServiceProvider();

    ServiceExecutor &consumer = provider->getService<ServiceExecutor>();

    std::cout << "work all: " << consumer.workAll() << std::endl;
    std::cout << "single work: " << provider->getService<IWorker>().work();

    return 0;
}