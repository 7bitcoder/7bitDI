#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{

    template <class T> class ExternalServiceFactory : public IServiceFactory
    {
        T *_service = nullptr;

      public:
        explicit ExternalServiceFactory(T *service) : _service{service} {}

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            return std::make_unique<services::ExternalService<T>>(_service);
        }
    };

} // namespace sb::di::details::factories
