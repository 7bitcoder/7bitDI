#include <SevenBit/DI.hpp>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>

using namespace sb::di;
using namespace std::chrono_literals;

struct IMessageBus
{
    virtual void send(const std::any &message) = 0;

    virtual void on(std::type_index typeId, std::function<void(const std::any &)> callback) = 0;

    template <class T, class F> void on(F f)
    {
        on(typeid(T), [f](const std::any &msg) { f(std::any_cast<const T &>(msg)); });
    }

    virtual ~IMessageBus() = default;
};

class MessageBus final : public IMessageBus
{
    std::unordered_map<std::type_index, std::vector<std::function<void(const std::any &)>>> _callbacks;

  public:
    void send(const std::any &message) override
    {
        if (const auto it = _callbacks.find(message.type()); it != _callbacks.end())
        {
            for (const auto &callback : it->second)
            {
                callback(message);
            }
        }
    }

    void on(const std::type_index typeId, std::function<void(const std::any &)> callback) override
    {
        _callbacks[typeId].emplace_back(std::move(callback));
    }
};

struct TickMessage
{
    int number;
    std::chrono::milliseconds delay;
};

class SenderService
{
    IMessageBus &_messageBus;

  public:
    explicit SenderService(IMessageBus &messageBus) : _messageBus(messageBus) {}

    void run(const int ticksCount, const std::chrono::milliseconds delay) const
    {
        for (int i = 1; i <= ticksCount; ++i)
        {
            const auto realDelay = sleepFor(delay);
            _messageBus.send(TickMessage{i, realDelay});
        }
    }

  private:
    [[nodiscard]] std::chrono::milliseconds sleepFor(const std::chrono::milliseconds delay) const
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(delay);
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
    }
};

class ReceiverService
{
    IMessageBus &_messageBus;

  public:
    explicit ReceiverService(IMessageBus &messageBus) : _messageBus(messageBus)
    {
        _messageBus.on<TickMessage>([&](const TickMessage &message) { onReceived(message); });
    }

    void onReceived(const TickMessage &message) const
    {
        std::cout << "Received tick message no: " << message.number << ", delay: " << message.delay.count() << "ms"
                  << std::endl;
    }
};

int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IMessageBus, MessageBus>()
                                   .addSingleton<SenderService>()
                                   .addSingleton<ReceiverService>()
                                   .buildServiceProvider();

    const auto &receiver = provider.getService<ReceiverService>(); // force construction
    const auto &sender = provider.getService<SenderService>();
    sender.run(5, 200ms);
    return 0;
}
