#pragma once

#include <memory>
#include <optional>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/IServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceProvider : public IServiceInstanceProvider
    {
        ServiceProviderOptions _options;
        IServiceInstanceProviderRoot &_root;
        containers::ServiceInstancesMap _scoped;
        ServiceProvider *_serviceProvider = nullptr;

        ServiceInstanceProvider(const ServiceInstanceProvider &provider);

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProvider>;

        explicit ServiceInstanceProvider(IServiceInstanceProviderRoot &root, ServiceProviderOptions options);

        ServiceInstanceProvider(ServiceInstanceProvider &&) noexcept = default;

        ServiceInstanceProvider &operator=(const ServiceInstanceProvider &) = delete;

        void init(ServiceProvider &serviceProvider) override;

        [[nodiscard]] IServiceInstanceProvider::Ptr createScope() const override;

        const IServiceInstance &getInstance(TypeId serviceTypeId) override;
        const IServiceInstance *tryGetInstance(TypeId serviceTypeId) override;
        const OneOrList<IServiceInstance::Ptr> *tryGetInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;
        IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstanceInPlace(TypeId serviceTypeId) override;
        IServiceInstance::Ptr tryCreateInstanceInPlace(TypeId serviceTypeId) override;

        void clear();

        [[nodiscard]] const ServiceProviderOptions &getOptions() const override;

      protected:
        const IServiceInstance *tryCreateAndRegister(const containers::ServiceDescriptorList &descriptors);
        const OneOrList<IServiceInstance::Ptr> *tryCreateAndRegisterAll(
            const containers::ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr tryCreate(const ServiceDescriptor &descriptor, bool inPlaceRequest);
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateAll(
            const containers::ServiceDescriptorList &descriptors);

        OneOrList<IServiceInstance::Ptr> &createRestInstances(const containers::ServiceDescriptorList &descriptors,
                                                              containers::ServiceInstanceList &instances,
                                                              bool inPlaceRequest);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);

        containers::ServiceInstancesMap *tryGetInstancesMap(const ServiceLifeTime &lifeTime);

        containers::ServiceInstanceList *findRegisteredInstances(TypeId serviceTypeId);

        [[nodiscard]] const containers::ServiceDescriptorList *findDescriptors(TypeId serviceTypeId) const;
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceProvider.hpp"
#endif
