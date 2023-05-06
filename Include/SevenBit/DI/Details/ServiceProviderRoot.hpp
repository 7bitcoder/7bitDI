#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServiceProvider.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
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
} // namespace sb::di::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceProviderRoot.hpp"
#endif
