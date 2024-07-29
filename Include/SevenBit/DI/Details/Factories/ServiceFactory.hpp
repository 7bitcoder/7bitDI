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
            if (inPlaceRequest)
            {
                return std::unique_ptr<InPlaceService<T>>(inject<InPlaceService<T>>(serviceProvider));
            }
            return std::unique_ptr<UniquePtrService<T>>(inject<UniquePtrService<T>>(serviceProvider));
        }

      private:
        template <class S> S *inject(ServiceProvider &serviceProvider) { return CtorInjector<S>{serviceProvider}(); }
    };

} // namespace sb::di::details
