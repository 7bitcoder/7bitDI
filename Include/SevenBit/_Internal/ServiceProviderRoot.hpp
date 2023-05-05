#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/IServiceProviderRoot.hpp"
#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServiceProvider.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    class ServiceProviderRoot : public ServiceProvider, public IServiceProviderRoot
    {
      public:
        using Ptr = std::unique_ptr<ServiceProviderRoot>;

      private:
        ServiceDescriptorsMap _descriptorsMap;
        ServicesMap _singletons;

      public:
        template <class TDescriptorIt>
        ServiceProviderRoot(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : ServiceProvider(*this, options), _descriptorsMap(begin, end), _singletons(options.strongDestructionOrder)
        {
            IServiceFactory::Ptr factory{
                new ExternalServiceFcnFactory{[](IServiceProvider &provider) { return &provider; }}};
            _descriptorsMap.add(
                ServiceDescriptor{typeid(IServiceProvider), ServiceLifeTime::scoped(), std::move(factory)});
            _descriptorsMap.seal();

            if (options.prebuildSingeletons)
            {
                prebuildSingeletons();
            }
        };

        const ServiceDescriptorsMap &getDescriptorsMap() override;

        ServicesMap &getSingletons() override;

        ~ServiceProviderRoot();

      private:
        void prebuildSingeletons();
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceProviderRoot.hpp"
#endif
