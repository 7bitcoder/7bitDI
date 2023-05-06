#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Details/CircularDependencyGuard.hpp"
#include "SevenBit/Details/ExternalServiceFactory.hpp"
#include "SevenBit/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/Details/IServiceProviderRoot.hpp"
#include "SevenBit/Details/ServiceDescriptorList.hpp"
#include "SevenBit/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/Details/ServicesMap.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceDescriber.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/TypeId.hpp"

namespace sb::details
{
    class ServiceProvider : public IServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

      private:
        IServiceProviderRoot &_root;
        ServiceProviderOptions _options;
        ServicesMap _services;
        CircularDependencyGuard _guard;

      public:
        ServiceProvider(IServiceProviderRoot &root, ServiceProviderOptions options);
        ServiceProvider(const ServiceProvider &) = delete;
        ServiceProvider(ServiceProvider &&) = delete;

        ServiceProvider &operator=(const ServiceProvider &) = delete;
        ServiceProvider &operator=(ServiceProvider &&) = delete;

        IServiceProvider::Ptr createScope() override;

        const IServiceInstance *tryGetInstance(TypeId serviceTypeId) override;

        const IServiceInstance &getInstance(TypeId serviceTypeId) override;

        std::vector<const IServiceInstance *> getInstances(TypeId serviceTypeId) override;

        IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) override;

        IServiceInstance::Ptr createInstance(TypeId serviceTypeId) override;

        std::vector<IServiceInstance::Ptr> createInstances(TypeId serviceTypeId) override;

        void clear();

      private:
        const IServiceInstance *createAndRegister(TypeId typeId);

        std::vector<const IServiceInstance *> createAndRegisterAll(TypeId typeId);

        IServiceInstance::Ptr create(TypeId typeId);

        std::vector<IServiceInstance::Ptr> createAll(TypeId typeId);

        const IServiceInstance *createAndRegister(const ServiceDescriptor &descriptor);

        std::vector<const IServiceInstance *> createAndRegisterAll(const ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr create(const ServiceDescriptor &descriptor);

        std::vector<IServiceInstance::Ptr> createAll(const ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor);

        const ServiceProviderOptions &getOptions();

        ServicesMap &getScoped();

        ServicesMap &getSingletons();

        const ServiceDescriptorsMap &getDescriptorsMap();
    };
} // namespace sb::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/Details/Impl/ServiceProvider.hpp"
#endif
