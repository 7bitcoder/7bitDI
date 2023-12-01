#pragma once

#include <memory>
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

        DefaultServiceProvider(DefaultServiceProvider &&) = default;

        DefaultServiceProvider &operator=(const DefaultServiceProvider &) = delete;
        DefaultServiceProvider &operator=(DefaultServiceProvider &&) = default;

        void clear();

        ServiceProvider::Ptr createScope() override;

      protected:
        const IServiceInstance &getInstance(TypeId serviceTypeId) override;

        const IServiceInstance *tryGetInstance(TypeId serviceTypeId) override;

        IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;

        std::vector<const IServiceInstance *> getInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;

        std::vector<IServiceInstance::Ptr> createInstances(TypeId serviceTypeId) override;

      private:
        const IServiceInstance *tryCreateAndRegisterMain(TypeId serviceTypeId);

        std::vector<const IServiceInstance *> createAndRegisterAll(TypeId typeId);

        const IServiceInstance *createAndRegisterMainFrom(const ServiceDescriptorList &descriptors);

        std::vector<const IServiceInstance *> createAndRegisterAllFrom(const ServiceDescriptorList &descriptors);

        std::vector<const IServiceInstance *> registerAll(TypeId serviceTypeId, ServiceList &services);

        ServiceList &fillServiceListFrom(const ServiceDescriptorList &descriptors, ServiceList &services);

        IServiceInstance::Ptr create(const ServiceDescriptor &descriptor);

        std::vector<IServiceInstance::Ptr> createAll(const ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr createInstanceFrom(const ServiceDescriptor &descriptor);

        const ServiceProviderOptions &getOptions();

        ServicesMap *tryGetServicesMapFor(const ServiceLifeTime &lifeTime);

        ServicesMap &getScoped();

        ServicesMap &getSingletons();

        const ServiceDescriptorsMap &getDescriptorsMap();

        void prebuildSingletons();
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/DefaultServiceProvider.hpp"
#endif
