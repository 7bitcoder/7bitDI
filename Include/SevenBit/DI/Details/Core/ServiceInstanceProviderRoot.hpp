#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/IServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstanceProviderRoot : public ServiceInstanceProvider, public IServiceInstanceProviderRoot
    {
        ServiceDescriptorsMap _descriptorsMap;
        ServiceInstancesMap _singletons;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProviderRoot>;
        using SPtr = std::shared_ptr<ServiceInstanceProviderRoot>;

        explicit ServiceInstanceProviderRoot(ServiceProviderOptions options = {});

        template <class TDescriptorIt>
        ServiceInstanceProviderRoot(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : ServiceInstanceProvider(*this, options),
              _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder)
        {
            _descriptorsMap.seal();
        }

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] const ServiceDescriptorsMap &getDescriptorsMap() const override;

        ServiceInstancesMap &getSingletons() override;

        ServiceInstancesCreator &getRootCreator() override;

      private:
        void prebuildSingletons();
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProviderRoot.hpp"
#endif
