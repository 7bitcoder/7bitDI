#include "SevenBit/DI/IServiceProvider.hpp"
#include <SevenBit/DI.hpp>
#include <iostream>

using namespace std;
using namespace sb::di;

struct Service
{
    string helloFromService() { return "Hello from service."; }
};

int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}.addSingleton<Service>().buildServiceProvider();

    Service &service = provider->getService<Service>();

    cout << service.helloFromService();

    return 0;
}