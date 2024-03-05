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

namespace sb::di::details
{
    class EXPORT ServiceInstanceProvider : public IServiceInstanceProvider
    {
        ServiceProviderOptions _options;
        ServiceInstanceCreator _instanceCreator;
        IServiceInstanceProviderRoot &_root;
        ServiceInstancesMap _scoped;

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
        ServiceInstanceList *findRegisteredInstances(TypeId serviceTypeId);

        [[nodiscard]] const ServiceDescriptorList *findDescriptors(TypeId serviceTypeId,
                                                                               bool transient) const;

        ServiceInstanceList *tryRegisterAndGet(const ServiceDescriptorList &descriptors,
                                                           std::optional<ServiceInstanceList> &&instances);

        std::optional<ServiceInstanceList> tryCreateNonTransient(
            const ServiceDescriptorList &descriptors);
        std::optional<ServiceInstanceList> tryCreateAllNonTransient(
            const ServiceDescriptorList &descriptors);
        ServiceInstanceList *createRestNonTransientAndGet(
            const ServiceDescriptorList &descriptors, ServiceInstanceList &instances);

        ServiceInstance tryCreateTransient(const ServiceDescriptorList &descriptors);
        std::optional<OneOrList<ServiceInstance>> tryCreateAllTransient(
            const ServiceDescriptorList &descriptors);

        ServiceInstancesResolver makeResolver(const ServiceDescriptorList &descriptors);
        ServiceInstanceCreator &getInstanceCreator();
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
