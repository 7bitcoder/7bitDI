#include <SevenBit/DI.hpp>
#include <iostream>

using namespace std;
using namespace sb::di;

struct IWorker
{
    virtual string work() = 0;

    virtual ~IWorker() = default;
};
class WorkerA final : public IWorker
{
  public:
    string work() { return "work A done!"; }
};
class WorkerB final : public IWorker
{
  public:
    string work() { return "work B done!"; }
};
class WorkerC final : public IWorker
{
  public:
    string work() { return "work C done!"; }
};

class ConsumerService
{
  private:
    vector<IWorker *> _workers;

  public:
    ConsumerService(vector<IWorker *> workers) { _workers = workers; }

    string workAll()
    {
        string result;
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
                                         .addSingleton<ConsumerService>()
                                         .buildServiceProvider();

    ConsumerService &consumer = provider->getService<ConsumerService>();

    cout << consumer.workAll() << endl;
    cout << "single work: " << provider->getService<IWorker>().work();

    return 0;
}