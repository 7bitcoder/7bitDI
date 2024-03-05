#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"

namespace sb::di::details
{

    template <class T> class ExternalServiceFactory final : public IServiceFactory
    {
        T *_service = nullptr;

      public:
        explicit ExternalServiceFactory(T *service) : _service{service} {}

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            return std::make_unique<ExternalService<T>>(_service);
        }
    };

} // namespace sb::di::details
