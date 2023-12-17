#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceProviderRoot : public ServiceInstanceProvider, public IServiceInstanceProviderRoot
    {
        containers::ServiceDescriptorsMap _descriptorsMap;
        containers::ServiceInstancesMap _singletons;
        ServiceProviderOptions _options;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProviderRoot>;
        using SPtr = std::shared_ptr<ServiceInstanceProviderRoot>;

        template <class TDescriptorIt>
        ServiceInstanceProviderRoot(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : ServiceInstanceProvider(static_cast<IServiceInstanceProviderRoot &>(*this)),
              _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder), _options(options)
        {
            _descriptorsMap.seal();
        }

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] const containers::ServiceDescriptorsMap &getDescriptorsMap() const override;

        containers::ServiceInstancesMap &getSingletons() override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest) override;

      private:
        void prebuildSingletons();
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProviderRoot.hpp"
#endif
