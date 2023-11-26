#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/CtorReflection.hpp"
#include "SevenBit/DI/Details/ServiceOwner.hpp"
#include "SevenBit/DI/Details/ServiceParamProvider.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    template <class T> class ServiceCtorFactory final : public IServiceFactory
    {
      private:
        using ConstructorTraits = ConstrutorTraits<T>;
        using Indices = std::make_index_sequence<ConstructorTraits::ArgsSize>;

      public:
        using Ptr = std::unique_ptr<ServiceCtorFactory>;

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider) const
        {
            return create(serviceProvider, Indices{});
        };

      private:
        template <size_t... Index>
        typename ServiceOwner<T>::Ptr create(ServiceProvider &serviceProvider, std::index_sequence<Index...>) const
        {
            auto servicePtr = std::make_unique<T>(
                ServiceParamProvider<typename ConstructorTraits::template Arg<Index>::Type>{}.getService(
                    serviceProvider)...);
            return std::make_unique<ServiceOwner<T>>(std::move(servicePtr));
        }
    };

} // namespace sb::di::details
