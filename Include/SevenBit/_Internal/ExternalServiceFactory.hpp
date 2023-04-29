#pragma once

#include <functional>
#include <memory>

#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceOwner2.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceProvider;

    template <class T> class ExternalServiceFactory final : public IServiceFactory
    {
      public:
        using FactoryFcn = std::function<T *(ServiceProvider &)>;

      private:
        T *_service;

      public:
        ExternalServiceFactory(T *service) : _service{service} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr get(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<T>>(_service);
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFactory<T>>(*this); }
    };

} // namespace sb