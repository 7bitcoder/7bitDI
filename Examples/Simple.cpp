#include "SevenBit/DI/Version.hpp"
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

struct AppPartA final : public IAppPartA
{
    void doPart() { std::cout << "part a done!"; }
};

struct AppPartB final : public IAppPartB
{
    void doPart() { std::cout << "part b done!"; }
};

class Application
{
    IAppPartA &_partA;
    IAppPartB &_partB;

  public:
    Application(IAppPartA *partA, IAppPartB *partB) : _partA(*partA), _partB(*partB) {}

    int run()
    {
        _partA.doPart();
        _partB.doPart();
        return 0;
    }
};
int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<IAppPartA, AppPartA>()
                                         .addSingleton<IAppPartB, AppPartB>()
                                         .addSingleton<Application>()
                                         .buildServiceProvider();

    Application &app = provider->getService<Application>();
    return app.run();
}