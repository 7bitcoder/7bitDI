#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/_Internal/CtorArgProvider.hpp"
#include "SevenBit/_Internal/CtorReflection.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceOwner2.hpp"

namespace sb
{
    class ServiceProvider;

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

        IServiceFactory::Ptr clone() { return std::make_unique<ServiceCtorFactory<T>>(*this); }

      private:
        template <size_t... Index>
        typename ServiceOwner2<T>::Ptr create(ServiceProvider &serviceProvider, std::index_sequence<Index...>) const
        {
            auto servicePtr =
                std::make_unique<T>(CtorArgProvider<typename ConstructorTraits::template Arg<Index>::Type>{}.getService(
                    serviceProvider)...);
            return std::make_unique<ServiceOwner2<T>>(std::move(servicePtr));
        }
    };

} // namespace sb