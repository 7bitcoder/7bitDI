#pragma once

#include <memory>
#include <optional>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceProvider : public IServiceInstanceProvider
    {
        ServiceProviderOptions _options;
        ServiceInstanceCreator _instanceCreator;
        IServiceInstanceProviderRoot &_root;
        containers::ServiceInstancesMap _scoped;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProvider>;

        ServiceInstanceProvider(IServiceInstanceProviderRoot &root, ServiceProviderOptions options);

        ServiceInstanceProvider(ServiceInstanceProvider &&) = delete;
        ServiceInstanceProvider(const ServiceInstanceProvider &) = delete;

        ServiceInstanceProvider &operator=(const ServiceInstanceProvider &) = delete;
        ServiceInstanceProvider &operator=(ServiceInstanceProvider &&) = delete;

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;

        [[nodiscard]] IServiceInstanceProvider::Ptr createScope() const override;

        const ServiceInstance &getInstance(TypeId serviceTypeId) override;
        const ServiceInstance *tryGetInstance(TypeId serviceTypeId) override;
        const OneOrList<ServiceInstance> *tryGetInstances(TypeId serviceTypeId) override;

        ServiceInstance createInstance(TypeId serviceTypeId) override;
        ServiceInstance tryCreateInstance(TypeId serviceTypeId) override;
        std::optional<OneOrList<ServiceInstance>> tryCreateInstances(TypeId serviceTypeId) override;

        ServiceInstance createInstanceInPlace(TypeId serviceTypeId) override;
        ServiceInstance tryCreateInstanceInPlace(TypeId serviceTypeId) override;

        void clear();

      protected:
        containers::ServiceInstanceList *findRegisteredInstances(TypeId serviceTypeId);

        [[nodiscard]] const containers::ServiceDescriptorList *findDescriptors(TypeId serviceTypeId,
                                                                               bool transient) const;

        containers::ServiceInstanceList *tryRegisterAndGet(const containers::ServiceDescriptorList &descriptors,
                                                           std::optional<containers::ServiceInstanceList> instances);

        std::optional<containers::ServiceInstanceList> tryCreate(const containers::ServiceDescriptorList &descriptors);
        std::optional<containers::ServiceInstanceList> tryCreateAll(
            const containers::ServiceDescriptorList &descriptors);
        containers::ServiceInstanceList *createRestAndGet(const containers::ServiceDescriptorList &descriptors,
                                                          containers::ServiceInstanceList &instances);

        ServiceInstance tryCreateAlias(const ServiceDescriptor &descriptor);
        std::optional<OneOrList<ServiceInstance>> tryCreateAliases(const ServiceDescriptor &descriptor);

        ServiceInstancesResolver makeResolver(const containers::ServiceDescriptorList &descriptors);
        ServiceInstanceCreator &getInstanceCreator();
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
