#include <SevenBit/DI.hpp>
#include <iostream>

using namespace sb::di;

struct IAppPartA
{
    virtual void doPart() = 0;

    virtual ~IAppPartA() = default;
};

struct IAppPartB
{
    virtual void doPart() = 0;

    virtual ~IAppPartB() = default;
};

struct AppPartA final : IAppPartA
{
    void doPart() override { std::cout << "part a done!" << std::endl; }
};

struct AppPartB final : IAppPartB
{
    void doPart() override { std::cout << "part b done!" << std::endl; }
};

class Application
{
    IAppPartA &_partA;
    IAppPartB &_partB;

  public:
    Application(IAppPartA *partA, IAppPartB *partB) : _partA(*partA), _partB(*partB) {}

    [[nodiscard]] int run() const
    {
        _partA.doPart();
        _partB.doPart();
        return 0;
    }
};
int main()
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IAppPartA, AppPartA>()
                                   .addSingleton<IAppPartB, AppPartB>()
                                   .addSingleton<Application>()
                                   .buildServiceProvider();

    const auto &app = provider.getService<Application>();
    return app.run();
}
