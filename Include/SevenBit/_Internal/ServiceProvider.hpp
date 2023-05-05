#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceDescriber.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/CircularDependencyGuard.hpp"
#include "SevenBit/_Internal/ExternalServiceFactory.hpp"
#include "SevenBit/_Internal/ExternalServiceFcnFactory.hpp"
#include "SevenBit/_Internal/IServiceProviderRoot.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
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

        void *getService(TypeId serviceTypeId) override;

        void *getRequiredService(TypeId serviceTypeId) override;

        std::vector<void *> getServices(TypeId serviceTypeId) override;

        void *createService(TypeId serviceTypeId) override;

        void *createRequiredService(TypeId serviceTypeId) override;

        std::vector<void *> createServices(TypeId serviceTypeId) override;

        void clear();

      private:
        void *createAndRegister(TypeId typeId);

        std::vector<void *> createAndRegisterAll(TypeId typeId);

        void *create(TypeId typeId);

        std::vector<void *> createAll(TypeId typeId);

        void *createAndRegister(const ServiceDescriptor &descriptor);

        std::vector<void *> createAndRegisterAll(const ServiceDescriptorList &descriptors);

        void *create(const ServiceDescriptor &descriptor);

        std::vector<void *> createAll(const ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor);

        const ServiceProviderOptions &getOptions();

        ServicesMap &getScoped();

        ServicesMap &getSingletons();

        const ServiceDescriptorsMap &getDescriptorsMap();
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceProvider.hpp"
#endif
