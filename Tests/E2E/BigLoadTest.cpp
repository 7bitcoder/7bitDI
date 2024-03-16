#include "../Helpers/Classes/Legion.hpp"

int main()
{
    constexpr size_t Size = 1000;
    sb::di::ServiceCollection services;
    addLegions<2000, Size>(services);

    sb::di::ServiceProviderOptions options;
    options.prebuildSingletons = true;
    auto provider = services.buildServiceProvider(options);

    return 0;
}
