#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceInstanceProviderData.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceProviderData : public IServiceInstanceProviderData
    {
        containers::ServiceDescriptorsMap _descriptorsMap;
        containers::ServiceInstancesMap _singletons;
        ServiceProviderOptions _options;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProviderData>;
        using SPtr = std::shared_ptr<ServiceInstanceProviderData>;

        template <class TDescriptorIt>
        ServiceInstanceProviderData(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder), _options(options)
        {
            _descriptorsMap.seal();
        }

        [[nodiscard]] const containers::ServiceDescriptorsMap &getDescriptorsMap() const override;

        containers::ServiceInstancesMap &getSingletons() override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProviderData.hpp"
#endif
