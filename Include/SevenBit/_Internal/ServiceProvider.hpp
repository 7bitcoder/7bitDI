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
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    class ServiceProvider final : public IServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

      private:
        ServiceProvider *_root = nullptr;
        ServiceDescriptorsMap::Ptr _descriptorsMap;
        ServiceProviderOptions _options;
        ServicesMap _services;
        CircularDependencyGuard _guard;

        ServiceProvider(ServiceProviderOptions options, ServiceProvider *root);

      public:
        template <class TDescriptorIt>
        ServiceProvider(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _descriptorsMap(std::make_unique<ServiceDescriptorsMap>(begin, end)), _options(std::move(options)),
              _services(_options.strongDestructionOrder), _root(this)
        {
            IServiceFactory::Ptr factory{
                new ExternalServiceFcnFactory{[](IServiceProvider &provider) { return &provider; }}};
            _descriptorsMap->add(
                ServiceDescriptor{typeid(IServiceProvider), ServiceLifeTime::scoped(), std::move(factory)});
            _descriptorsMap->seal();
        }

        ServiceProvider(const ServiceProvider &) = delete;
        ServiceProvider(ServiceProvider &&) = delete;

        ServiceProvider &operator=(const ServiceProvider &) = delete;
        ServiceProvider &operator=(ServiceProvider &&) = delete;

        IServiceProvider::Ptr createScope();

        void *getService(TypeId serviceTypeId) final;

        void *getRequiredService(TypeId serviceTypeId);

        std::vector<void *> getServices(TypeId serviceTypeId) final;

        void *createService(TypeId serviceTypeId);

        void *createRequiredService(TypeId serviceTypeId);

        std::vector<void *> createServices(TypeId serviceTypeId);

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

        const ServiceDescriptorsMap &getDescriptorsMap();

        ServicesMap &scoped();

        ServicesMap &singletons();
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceProvider.hpp"
#endif
