#include "SevenBitRest.hpp"

using namespace std;
using namespace sb;

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
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addScoped<IWorker, WorkerA>();
    services.addScoped<IWorker, WorkerB>();
    services.addScoped<IWorker, WorkerC>();
    services.addScoped<ConsumerService>();

    auto rest = builder.build();

    rest.mapGet("/", [](ConsumerService &consumer) { return consumer.workAll(); });

    rest.run();
}