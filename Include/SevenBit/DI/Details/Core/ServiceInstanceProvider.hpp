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

        [[nodiscard]] const ServiceProviderOptions &getOptions() const { return _options; }

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] IServiceInstanceProvider::Ptr createScope() const override;

        const ServiceInstance &getInstance(const TypeId serviceTypeId) override
        {
            return getInstance(ServiceId{serviceTypeId});
        }
        const ServiceInstance *tryGetInstance(const TypeId serviceTypeId) override
        {
            return tryGetInstance(ServiceId{serviceTypeId});
        }
        const OneOrList<ServiceInstance> *tryGetInstances(const TypeId serviceTypeId) override
        {
            return tryGetInstances(ServiceId{serviceTypeId});
        }

        ServiceInstance createInstance(const TypeId serviceTypeId) override
        {
            return createInstance(ServiceId{serviceTypeId});
        }
        ServiceInstance tryCreateInstance(const TypeId serviceTypeId) override
        {
            return tryCreateInstance(ServiceId{serviceTypeId});
        }
        std::optional<OneOrList<ServiceInstance>> tryCreateInstances(const TypeId serviceTypeId) override
        {
            return tryCreateInstances(ServiceId{serviceTypeId});
        }

        ServiceInstance createInstanceInPlace(const TypeId serviceTypeId) override
        {
            return createInstanceInPlace(ServiceId{serviceTypeId});
        }
        ServiceInstance tryCreateInstanceInPlace(const TypeId serviceTypeId) override
        {
            return tryCreateInstanceInPlace(ServiceId{serviceTypeId});
        }

        const ServiceInstance &getKeyedInstance(const TypeId serviceTypeId, const std::string_view serviceKey) override
        {
            return getInstance(ServiceId{serviceTypeId, serviceKey});
        }
        const ServiceInstance *tryGetKeyedInstance(const TypeId serviceTypeId,
                                                   const std::string_view serviceKey) override
        {
            return tryGetInstance(ServiceId{serviceTypeId, serviceKey});
        }
        const OneOrList<ServiceInstance> *tryGetKeyedInstances(const TypeId serviceTypeId,
                                                               const std::string_view serviceKey) override
        {
            return tryGetInstances(ServiceId{serviceTypeId, serviceKey});
        }

        ServiceInstance createKeyedInstance(const TypeId serviceTypeId, const std::string_view serviceKey) override
        {
            return createInstance(ServiceId{serviceTypeId, serviceKey});
        }
        ServiceInstance tryCreateKeyedInstance(const TypeId serviceTypeId, const std::string_view serviceKey) override
        {
            return tryCreateInstance(ServiceId{serviceTypeId, serviceKey});
        }
        std::optional<OneOrList<ServiceInstance>> tryCreateKeyedInstances(const TypeId serviceTypeId,
                                                                          const std::string_view serviceKey) override
        {
            return tryCreateInstances(ServiceId{serviceTypeId, serviceKey});
        }

        ServiceInstance createKeyedInstanceInPlace(const TypeId serviceTypeId,
                                                   const std::string_view serviceKey) override
        {
            return createInstanceInPlace(ServiceId{serviceTypeId, serviceKey});
        }
        ServiceInstance tryCreateKeyedInstanceInPlace(const TypeId serviceTypeId,
                                                      const std::string_view serviceKey) override
        {
            return tryCreateInstanceInPlace(ServiceId{serviceTypeId, serviceKey});
        }

      protected:
        const ServiceInstance &getInstance(const ServiceId &id);
        const ServiceInstance *tryGetInstance(const ServiceId &id);
        const OneOrList<ServiceInstance> *tryGetInstances(const ServiceId &id);

        ServiceInstance createInstance(const ServiceId &id);
        ServiceInstance tryCreateInstance(const ServiceId &id);
        std::optional<OneOrList<ServiceInstance>> tryCreateInstances(const ServiceId &id);

        ServiceInstance createInstanceInPlace(const ServiceId &id);
        ServiceInstance tryCreateInstanceInPlace(const ServiceId &id);

        void clear() { _scoped.clear(); }

        ServiceInstanceList *findRegisteredInstances(const ServiceId &id);

        [[nodiscard]] const ServiceDescriptorList *findDescriptors(const ServiceId &id, bool transient) const;

        ServiceInstanceList *tryRegisterAndGet(const ServiceId &id, const ServiceDescriptorList &descriptors,
                                               std::optional<ServiceInstanceList> &&instances);

        std::optional<ServiceInstanceList> tryCreateNonTransient(const ServiceDescriptorList &descriptors);
        std::optional<ServiceInstanceList> tryCreateAllNonTransient(const ServiceDescriptorList &descriptors);
        ServiceInstanceList *createRestNonTransient(const ServiceDescriptorList &descriptors,
                                                          ServiceInstanceList &instances);

        ServiceInstance tryCreateTransient(const ServiceDescriptorList &descriptors);
        std::optional<OneOrList<ServiceInstance>> tryCreateAllTransient(const ServiceDescriptorList &descriptors);

        ServiceInstancesResolver makeResolver(const ServiceDescriptorList &descriptors);

        ServiceInstanceCreator &getInstanceCreator() { return _instanceCreator; }
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
