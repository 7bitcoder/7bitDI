#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/IServiceProviderCore.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    class EXPORT DefaultServiceProviderCore : public IServiceProviderCore
    {
      private:
        ServiceDescriptorsMap _descriptorsMap;
        ServicesMap _singletons;
        ServiceProviderOptions _options;

      public:
        using Ptr = std::unique_ptr<DefaultServiceProviderCore>;
        using SPtr = std::shared_ptr<DefaultServiceProviderCore>;

        template <class TDescriptorIt>
        DefaultServiceProviderCore(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _options(options), _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder)
        {
            IServiceFactory::Ptr factory{
                new ExternalServiceFcnFactory{[](ServiceProvider &provider) { return &provider; }}};
            _descriptorsMap.add(
                ServiceDescriptor{typeid(ServiceProvider), ServiceLifeTime::scoped(), std::move(factory)});
            _descriptorsMap.seal();
        }

        [[nodiscard]] const ServiceDescriptorsMap &getDescriptorsMap() const override;

        ServicesMap &getSingletons() override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/DefaultServiceProviderCore.hpp"
#endif
