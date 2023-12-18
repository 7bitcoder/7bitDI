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
    class EXPORT ServiceInstanceProviderRoot : public ServiceInstanceProvider
    {
        containers::ServiceDescriptorsMap _descriptorsMap;
        containers::ServiceInstancesMap _singletons;
        helpers::CircularDependencyGuard _guard;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProviderRoot>;
        using SPtr = std::shared_ptr<ServiceInstanceProviderRoot>;

        template <class TDescriptorIt>
        ServiceInstanceProviderRoot(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : ServiceInstanceProvider(*this, options),
              _descriptorsMap(begin, end, options.checkServiceGlobalUniqueness),
              _singletons(options.strongDestructionOrder)
        {
            _descriptorsMap.seal();
        }

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] const containers::ServiceDescriptorsMap &getDescriptorsMap() const;

        containers::ServiceInstancesMap &getSingletons();

        helpers::ScopedGuard spawnGuard(TypeId typeId);

      private:
        void prebuildSingletons();
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProviderRoot.hpp"
#endif
