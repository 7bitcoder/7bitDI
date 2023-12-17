#include <benchmark/benchmark.h>

#include "Classes.hpp"
#include <SevenBit/DI.hpp>

class ServiceProviderBenchmark : public benchmark::Fixture
{
  public:
    sb::di::ServiceProvider _provider = make();

    void SetUp(const ::benchmark::State &state) {}

    void TearDown(const ::benchmark::State &state) {}

    sb::di::ServiceProvider make()
    {
        return sb::di::ServiceCollection()
            .addSingleton<ITestComplexClass1, TestComplexClass1>()
            .addSingleton<ITestComplexClass1, TestComplexClass12>()
            .addSingleton<ITestComplexClass1, TestComplexClass13>()
            .addSingleton<ITestComplexClass2, TestComplexClass2>()
            .addTransient<ITestComplexClass3, TestComplexClass3>()
            .addTransient<TestComplexClass31>()
            .addScoped<ITestComplexClass4, TestComplexClass4>()
            .addScoped<ITestComplexClass5, TestComplexClass5>()
            .addScoped<ITestComplexClass6, TestComplexClass6>()
            .buildServiceProvider();
    }
};

BENCHMARK_F(ServiceProviderBenchmark, GetServiceSimple)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<ITestComplexClass1>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServiceOneRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<ITestComplexClass2>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServiceUniqueRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<ITestComplexClass4>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServiceManyRefs)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<ITestComplexClass5>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServiceVecRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<ITestComplexClass6>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, CreateService)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.createService<ITestComplexClass3>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, CreateServicesInPlace)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.createServiceInPlace<TestComplexClass31>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceSimple)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass1>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceOneRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass2>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceUnique)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass3>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceUniqueRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass4>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceManyRefs)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass5>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetServiceVecRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<ITestComplexClass6>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesSimple)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass1>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesOneRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass2>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesUnique)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass3>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesUniqueRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass4>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesManyRefs)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass5>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, GetServicesVecRef)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getServices<ITestComplexClass6>();
    }
}

BENCHMARK_MAIN();
