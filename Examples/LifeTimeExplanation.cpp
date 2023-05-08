#include "SevenBit/DI/IServiceProvider.hpp"
#include <SevenBit/DI.hpp>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std;
using namespace std::string_literals;
using namespace sb::di;

struct SingletonService
{
};
struct ScopedService
{
};
struct TransientService
{
};

template <class T> string objectInfo(const T &ptr)
{
    stringstream stream;
    stream << '\t' << typeid(T).name() << " pointer: " << &ptr << endl;
    return stream.str();
}

string getServicesInfo(IServiceProvider &provider)
{
    SingletonService &singleton = provider.getService<SingletonService>();
    ScopedService &scoped = provider.getService<ScopedService>();
    std::unique_ptr<TransientService> transient = provider.createService<TransientService>();

    return objectInfo(singleton) + objectInfo(scoped) + objectInfo(*transient);
};

int main()
{
    IServiceProvider::Ptr provider = ServiceCollection{}
                                         .addSingleton<SingletonService>()
                                         .addScoped<ScopedService>()
                                         .addTransient<TransientService>()
                                         .buildServiceProvider();

    cout << "root provider: \n";
    cout << getServicesInfo(*provider) << endl;
    cout << getServicesInfo(*provider) << endl;

    IServiceProvider::Ptr scope1 = provider->createScope();
    cout << "scope1 provider: \n";
    cout << getServicesInfo(*scope1) << endl;
    cout << getServicesInfo(*scope1) << endl;

    IServiceProvider::Ptr scope2 = provider->createScope();
    cout << "scope2 provider: \n";
    cout << getServicesInfo(*scope2) << endl;
    cout << getServicesInfo(*scope2) << endl;
}