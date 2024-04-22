#pragma once

#include <memory>
#include <optional>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Core/ServiceAliasesCreator.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstanceProvider : public IServiceInstanceProvider
    {
        ServiceProviderOptions _options;
        ServiceInstancesCreator _instancesCreator;
        ServiceAliasesCreator _aliasesCreator;
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
        OneOrList<ServiceInstance> tryCreateInstances(const TypeId serviceTypeId) override
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
        OneOrList<ServiceInstance> tryCreateKeyedInstances(const TypeId serviceTypeId,
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
        OneOrList<ServiceInstance> tryCreateInstances(const ServiceId &id);

        ServiceInstance createInstanceInPlace(const ServiceId &id);
        ServiceInstance tryCreateInstanceInPlace(const ServiceId &id);

        void clear() { _scoped.clear(); }

        ServiceInstanceList *findInstances(const ServiceId &id);

        [[nodiscard]] const ServiceDescriptorList *findDescriptors(const ServiceId &id) const;
        [[nodiscard]] const ServiceDescriptorList *findTransientDescriptors(const ServiceId &id) const;

        ServiceLifeTime getLifeTime(const ServiceDescriptor &descriptor) const;
        ServiceInstancesMap &getInstancesMap(ServiceLifeTime lifetime);

        ServiceInstance tryCreateInstance(const ServiceDescriptor &descriptor);
        ServiceInstanceList tryCreateInstances(const ServiceDescriptorList &descriptors);
        void createRestInstances(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances);

        ServiceInstance tryCreateTransientInstance(const ServiceDescriptor &descriptor);
        ServiceInstanceList tryCreateTransientInstances(const ServiceDescriptorList &descriptors);

        ServiceInstancesCreator &selectCreator(const ServiceDescriptor &descriptor);
        ServiceAliasesCreator &getAliasesCreator() { return _aliasesCreator; }
        ServiceInstancesCreator &getCreator() { return _instancesCreator; }
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
