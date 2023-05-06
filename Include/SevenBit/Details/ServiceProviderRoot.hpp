#pragma once

#include <memory>
#include <vector>

#include "SevenBit/Details/IServiceProviderRoot.hpp"
#include "SevenBit/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/Details/ServiceProvider.hpp"
#include "SevenBit/Details/ServicesMap.hpp"
#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"

namespace sb::details
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
} // namespace sb::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/Details/Impl/ServiceProviderRoot.hpp"
#endif
