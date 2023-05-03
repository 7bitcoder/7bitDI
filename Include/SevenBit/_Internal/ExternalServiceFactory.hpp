#pragma once

#include <memory>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ExternalService.hpp"

namespace sb::internal
{

    template <class T> class ExternalServiceFactory final : public IServiceFactory
    {
      private:
        T *_service = nullptr;

      public:
        ExternalServiceFactory(T *service) : _service{service} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<T>>(_service);
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFactory<T>>(*this); }
    };

} // namespace sb::internal