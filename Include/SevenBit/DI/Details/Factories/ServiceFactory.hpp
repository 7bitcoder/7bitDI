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
      private:
        using Injector = CtorInjector<T>;

      public:
        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            Injector injector{serviceProvider};
            if (inPlaceRequest)
            {
                return injector([](auto... ctorParams) { return std::make_unique<InPlaceService<T>>(ctorParams...); });
            }
            return injector([](auto... ctorParams) { return std::make_unique<UniquePtrService<T>>(ctorParams...); });
        }
    };

} // namespace sb::di::details
