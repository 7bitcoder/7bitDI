#include "../Helpers/Classes/Legion.hpp"

int main()
{
    sb::di::ServiceCollection services;
    addLegions<1000>(services);

    sb::di::ServiceProviderOptions options;
    options.prebuildSingletons = true;
    auto provider = services.buildServiceProvider(options);

    return 0;
}
