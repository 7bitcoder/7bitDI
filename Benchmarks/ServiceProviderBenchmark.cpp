#include <SevenBit/DI.hpp>
#include <benchmark/benchmark.h>

class Test
{
};

class ServiceProviderBenchmark : public benchmark::Fixture
{
  public:
    sb::di::ServiceProvider _provider = make();

    void SetUp(const ::benchmark::State &state) {}

    void TearDown(const ::benchmark::State &state) {}

    sb::di::ServiceProvider make() { return sb::di::ServiceCollection().addSingleton<Test>().buildServiceProvider(); }
};

BENCHMARK_F(ServiceProviderBenchmark, GetService)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.getService<Test>();
    }
}

BENCHMARK_F(ServiceProviderBenchmark, TryGetService)(benchmark::State &st)
{
    for (auto _ : st)
    {
        _provider.tryGetService<Test>();
    }
}

BENCHMARK_MAIN();
