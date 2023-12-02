#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/CircularDependencyGuard.hpp"
#include "SevenBit/DI/Details/IServiceProviderCore.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT DefaultServiceProvider : public ServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<DefaultServiceProvider>;

      private:
        IServiceProviderCore::SPtr _sharedCore;
        ServicesMap _scoped;
        CircularDependencyGuard _guard;

        DefaultServiceProvider(const DefaultServiceProvider &);

      public:
        explicit DefaultServiceProvider(IServiceProviderCore::Ptr core);

        DefaultServiceProvider(DefaultServiceProvider &&) noexcept = default;

        DefaultServiceProvider &operator=(const DefaultServiceProvider &) = delete;
        DefaultServiceProvider &operator=(DefaultServiceProvider &&) = default;

        void clear();

        ServiceProvider::Ptr createScope() override;

      protected:
        const IServiceInstance *tryGetInstance(TypeId serviceTypeId) override;
        const IServiceInstance &getInstance(TypeId serviceTypeId) override;
        const OneOrList<IServiceInstance::Ptr> *tryGetInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;
        IServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateInstances(TypeId serviceTypeId) override;

      private:
        const IServiceInstance *tryCreateAndRegister(const ServiceDescriptorList &descriptors);
        const ServiceList *tryCreateAndRegisterAll(const ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr tryCreate(const ServiceDescriptorList &descriptors);
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateAll(const ServiceDescriptorList &descriptors);

        ServiceList &fillServicesWithAll(const ServiceDescriptorList &descriptors, ServiceList &services);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor);

        ServicesMap *tryGetServicesMap(const ServiceLifeTime &lifeTime);

        ServiceList *findRegisteredServices(TypeId serviceTypeId);

        const ServiceDescriptorList *findDescriptors(TypeId serviceTypeId) const;

        const ServiceProviderOptions &getOptions();

        void prebuildSingletons();
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/DefaultServiceProvider.hpp"
#endif
