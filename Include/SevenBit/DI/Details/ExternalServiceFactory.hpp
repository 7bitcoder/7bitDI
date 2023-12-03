#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{

    template <class T> class ExternalServiceFactory final : public IServiceFactory
    {
      private:
        T *_service = nullptr;

      public:
        ExternalServiceFactory(T *service) : _service{service} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlace) const
        {
            return std::make_unique<ExternalService<T>>(_service);
        }
    };

} // namespace sb::di::details
