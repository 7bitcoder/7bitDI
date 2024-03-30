#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/CtorInjector.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"

namespace sb::di::details
{
    template <class T> class ServiceFactory final : public IServiceFactory
    {
        struct InPlaceCreator
        {
            template <class... Args> IServiceInstance::Ptr operator()(Args &&...params)
            {
                return std::make_unique<InPlaceService<T>>(std::forward<Args>(params)...);
            }
        };

        struct UniqueCreator
        {
            template <class... Args> IServiceInstance::Ptr operator()(Args &&...params)
            {
                auto servicePtr = std::make_unique<T>(std::forward<Args>(params)...);
                return std::make_unique<UniquePtrService<T>>(std::move(servicePtr));
            }
        };

      public:
        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            CtorInjector<T> injector{serviceProvider};
            if (inPlaceRequest)
            {
                return injector.callWithCtorArgs(InPlaceCreator{});
            }
            return injector.callWithCtorArgs(UniqueCreator{});
        }
    };

} // namespace sb::di::details
