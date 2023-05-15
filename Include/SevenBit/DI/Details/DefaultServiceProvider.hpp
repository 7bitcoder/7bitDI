#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/CircularDependencyGuard.hpp"
#include "SevenBit/DI/Details/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT DefaultServiceProvider : public IServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<DefaultServiceProvider>;

      private:
        IServiceProviderRoot &_root;
        ServiceProviderOptions _options;
        ServicesMap _services;
        CircularDependencyGuard _guard;

      public:
        DefaultServiceProvider(IServiceProviderRoot &root, ServiceProviderOptions options);
        DefaultServiceProvider(const DefaultServiceProvider &) = delete;
        DefaultServiceProvider(DefaultServiceProvider &&) = delete;

        DefaultServiceProvider &operator=(const DefaultServiceProvider &) = delete;
        DefaultServiceProvider &operator=(DefaultServiceProvider &&) = delete;

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
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/DefaultServiceProvider.hpp"
#endif
