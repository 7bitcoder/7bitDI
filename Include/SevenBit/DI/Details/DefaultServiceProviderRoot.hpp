#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/Details/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    class EXPORT DefaultServiceProviderRoot : public DefaultServiceProvider, public IServiceProviderRoot
    {
      public:
        using Ptr = std::unique_ptr<DefaultServiceProviderRoot>;

      private:
        ServiceDescriptorsMap _descriptorsMap;
        ServicesMap _singletons;

      public:
        template <class TDescriptorIt>
        DefaultServiceProviderRoot(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : DefaultServiceProvider(*this, options), _descriptorsMap(begin, end),
              _singletons(options.strongDestructionOrder)
        {
            IServiceFactory::Ptr factory{
                new ExternalServiceFcnFactory{[](ServiceProvider &provider) { return &provider; }}};
            _descriptorsMap.add(
                ServiceDescriptor{typeid(ServiceProvider), ServiceLifeTime::scoped(), std::move(factory)});
            _descriptorsMap.seal();

            if (options.prebuildSingeletons)
            {
                prebuildSingeletons();
            }
        };

        const ServiceDescriptorsMap &getDescriptorsMap() override;

        ServicesMap &getSingletons() override;

        ~DefaultServiceProviderRoot();

      private:
        void prebuildSingeletons();
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/DefaultServiceProviderRoot.hpp"
#endif
