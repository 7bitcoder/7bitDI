#include "SevenBitRest.hpp"

using namespace std;
using namespace std::string_literals;
using namespace sb::di;

std::string tabs(size_t indentation) { return string(indentation, '\t'); }

template <class T> string objectInfo(T *ptr, size_t indentation = 1)
{
    stringstream stream;
    stream << tabs(indentation) << typeid(T).name() << " pointer: " << ptr << std::endl;
    return stream.str();
}

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

class ConsumerAService
{
  private:
    SingletonService *_singleton;
    ScopedService *_scoped;
    unique_ptr<TransientService> _transient;

  public:
    ConsumerAService(SingletonService *singleton, ScopedService *scoped, unique_ptr<TransientService> transient)
    {
        _singleton = singleton;
        _scoped = scoped;
        _transient = std::move(transient);
    }

    string info(size_t indentation = 1)
    {
        return objectInfo(_singleton) + objectInfo(_scoped) + objectInfo(_transient.get());
    }
};

class ConsumerBService
{
  private:
    SingletonService *_singleton;
    ScopedService *_scoped;
    unique_ptr<TransientService> _transient;

  public:
    ConsumerBService(SingletonService *singleton, ScopedService *scoped, unique_ptr<TransientService> transient)
    {
        _singleton = singleton;
        _scoped = scoped;
        _transient = std::move(transient);
    }

    string info(size_t indentation = 1)
    {
        return objectInfo(_singleton) + objectInfo(_scoped) + objectInfo(_transient.get());
    }
};

int main()
{
    WebApplicationBuilder builder;

    auto &services = builder.getServices();

    services.addSingleton<SingletonService>();
    services.addScoped<ScopedService>();
    services.addTransient<TransientService>();
    services.addScoped<ConsumerAService>();
    services.addScoped<ConsumerBService>();

    auto rest = builder.build();

    rest.mapGet("/", [](SingletonService &singleton, ScopedService &scoped, unique_ptr<TransientService> transient,
                        ConsumerAService &consumerA, ConsumerBService &consumerB) {
        auto fromEndpoint =
            "From endpoint:\n" + objectInfo(&singleton) + objectInfo(&scoped) + objectInfo(transient.get());
        auto fromConsumerA = "From consumer A:\n" + consumerA.info();
        auto fromConsumerB = "From consumer B:\n" + consumerB.info();

        return fromEndpoint + '\n' + fromConsumerA + '\n' + fromConsumerB + '\n';
    });

    rest.run();
}