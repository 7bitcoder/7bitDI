#pragma once

#include <memory>
#include <optional>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
    class ServiceInstanceProviderRoot;

    class EXPORT ServiceInstanceProvider : public IServiceInstanceProvider
    {
        ServiceProviderOptions _options;
        ServiceInstanceCreator _instanceCreator;
        ServiceInstanceProviderRoot &_root;
        containers::ServiceInstancesMap _scoped;

        using ServiceDescriptorList = containers::ServiceDescriptorList;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProvider>;

        ServiceInstanceProvider(ServiceInstanceProviderRoot &root, ServiceProviderOptions options);

        ServiceInstanceProvider(ServiceInstanceProvider &&) noexcept = delete;
        ServiceInstanceProvider(const ServiceInstanceProvider &) = delete;

        ServiceInstanceProvider &operator=(const ServiceInstanceProvider &) = delete;
        ServiceInstanceProvider &operator=(ServiceInstanceProvider &&) = delete;

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;

        [[nodiscard]] IServiceInstanceProvider::Ptr createScope() const override;

        const ServiceInstance &getInstance(TypeId serviceTypeId) override;
        const ServiceInstance *tryGetInstance(TypeId serviceTypeId) override;
        const OneOrList<ServiceInstance::Ptr> *tryGetInstances(TypeId serviceTypeId) override;

        ServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;
        ServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;
        std::optional<OneOrList<ServiceInstance::Ptr>> tryCreateInstances(TypeId serviceTypeId) override;

        ServiceInstance::Ptr createInstanceInPlace(TypeId serviceTypeId) override;
        ServiceInstance::Ptr tryCreateInstanceInPlace(TypeId serviceTypeId) override;

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

        ServiceInstancesResolver makeResolver(const containers::ServiceDescriptorList &descriptors);

        void tryAddOffset(ServiceInstance *instance, const ServiceDescriptor &descriptor);
        void tryAddOffset(std::optional<OneOrList<ServiceInstance::Ptr>> &instances,
                          const ServiceDescriptor &descriptor);
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
