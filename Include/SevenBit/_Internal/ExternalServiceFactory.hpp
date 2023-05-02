#pragma once

#include <functional>
#include <memory>

#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{

    template <class T> class ExternalServiceFactory final : public IServiceFactory
    {
      private:
        T *_service;

      public:
        ExternalServiceFactory(T *service) : _service{service} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr get(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<T>>(_service);
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFactory<T>>(*this); }
    };

} // namespace sb