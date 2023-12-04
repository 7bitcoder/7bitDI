#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/CtorReflection.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::factories
{
    template <class T> class ServiceCtorFactory : public IServiceFactory
    {
      private:
        using ConstructorTraits = utils::ConstructorTraits<T>;

      public:
        using Ptr = std::unique_ptr<ServiceCtorFactory>;

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override
        {
            return create(serviceProvider, std::make_index_sequence<ConstructorTraits::ArgsSize>{}, inPlaceRequest);
        };

      private:
        template <size_t... Index>
        typename services::InPlaceService<T>::Ptr create(ServiceProvider &serviceProvider,
                                                         std::index_sequence<Index...>, bool inPlaceRequest) const
        {
            if (inPlaceRequest)
            {
                return std::make_unique<services::InPlaceService<T>>(
                    helpers::ServiceParamProvider<typename ConstructorTraits::template Arg<Index>::Type>{}.getService(
                        serviceProvider)...);
            }
            auto servicePtr = std::make_unique<T>(
                helpers::ServiceParamProvider<typename ConstructorTraits::template Arg<Index>::Type>{}.getService(
                    serviceProvider)...);
            return std::make_unique<services::UniquePtrService<T>>(std::move(servicePtr));
        }
    };

} // namespace sb::di::details::factories
