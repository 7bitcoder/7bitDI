#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceCtorInvoker.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/CtorReflection.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::factories
{
    template <class T> class ServiceFactory : public IServiceFactory
    {
      public:
        using ServiceCtorInvoker = helpers::ServiceCtorInvoker<T>;

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override
        {
            ServiceCtorInvoker invoker{serviceProvider};
            if (inPlaceRequest)
            {
                return invoker.invokeWithCtorParams([](auto &&...params) -> IServiceInstance::Ptr {
                    return std::make_unique<services::InPlaceService<T>>(std::forward<decltype(params)>(params)...);
                });
            }
            return invoker.invokeWithCtorParams([](auto &&...params) -> IServiceInstance::Ptr {
                auto servicePtr = std::make_unique<T>(std::forward<decltype(params)>(params)...);
                return std::make_unique<services::UniquePtrService<T>>(std::move(servicePtr));
            });
        }
    };

} // namespace sb::di::details::factories
