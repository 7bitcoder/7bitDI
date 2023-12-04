#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceProviderData.hpp"
#include "SevenBit/DI/Details/Factories/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    class EXPORT DefaultServiceProviderData : public IServiceProviderData
    {
      private:
        containers::ServiceDescriptorsMap _descriptorsMap;
        containers::ServiceInstancesMap _singletons;
        ServiceProviderOptions _options;

      public:
        using Ptr = std::unique_ptr<DefaultServiceProviderData>;
        using SPtr = std::shared_ptr<DefaultServiceProviderData>;

        template <class TDescriptorIt>
        DefaultServiceProviderData(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _options(options), _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder)
        {
            IServiceFactory::Ptr factory{
                new factories::ExternalServiceFcnFactory{[](ServiceProvider &provider) { return &provider; }}};
            _descriptorsMap.add(
                ServiceDescriptor{typeid(ServiceProvider), ServiceLifeTime::scoped(), std::move(factory)});
            _descriptorsMap.seal();
        }

        [[nodiscard]] const containers::ServiceDescriptorsMap &getDescriptorsMap() const override;

        containers::ServiceInstancesMap &getSingletons() override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/DefaultServiceProviderCore.hpp"
#endif
