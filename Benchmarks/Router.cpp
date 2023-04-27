#include <benchmark/benchmark.h>

#include "Engine/Endpoint.hpp"
#include "Engine/IContext.hpp"
#include "Http/HttpMethod.hpp"
#include "Router/Router.hpp"

static void RouterBenchmark(benchmark::State &state)
{
    sb::Router router;
    router.addEndpoint(
        std::make_unique<sb::Endpoint>(sb::HttpMethod::Get, "/api/users/{id:int}", [](sb::IContext &) {}));
    router.addEndpoint(std::make_unique<sb::Endpoint>(sb::HttpMethod::Get, "/api/users/{id:int}/settings/email",
                                                      [](sb::IContext &) {}));
    router.addEndpoint(
        std::make_unique<sb::Endpoint>(sb::HttpMethod::Post, "/api/users/{id:int}/settings", [](sb::IContext &) {}));
    router.addEndpoint(std::make_unique<sb::Endpoint>(sb::HttpMethod::Get, "/api/users", [](sb::IContext &) {}));

    router.compile();

    for (auto _ : state)
        router.match(sb::HttpMethod::Get, "/api/users/12");
}

BENCHMARK(RouterBenchmark)->Complexity()->MeasureProcessCPUTime();

BENCHMARK_MAIN();