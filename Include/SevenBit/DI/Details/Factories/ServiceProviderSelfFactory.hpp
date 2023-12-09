#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{

    class ServiceProviderSelfFactory : public IServiceFactory
    {
      public:
        [[nodiscard]] TypeId getServiceTypeId() const override;

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override;
    };

} // namespace sb::di::details::factories

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Factories/Impl/ServiceProviderSelfFactory.hpp"
#endif
