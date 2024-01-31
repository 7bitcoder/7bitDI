#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{

    template <class T> class VoidServiceFactory final : public IServiceFactory
    {
      public:
        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(T); }

        ServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            return nullptr;
        }
    };

} // namespace sb::di::details::factories
