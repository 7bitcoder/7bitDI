#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/_Internal/CtorArgProvider.hpp"
#include "SevenBit/_Internal/CtorReflection.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"

namespace sb
{
    class ServiceProvider;

    template <class I, class T> class ServiceConstructor final : public IServiceCreator
    {
      private:
        using ConstructorTraits = ConstrutorTraits<T>;
        using Indices = std::make_index_sequence<ConstructorTraits::ArgsSize>;
        ServiceLifeTime _scope;

      public:
        using Ptr = std::unique_ptr<ServiceConstructor>;
        ServiceConstructor(const ServiceLifeTime &scope) : _scope{scope}
        {
            static_assert(std::is_base_of_v<I, T>, "Type T must inherit from I");
        }

        const ServiceLifeTime &getServiceScope() const { return _scope; };

        TypeId getServiceTypeId() const { return typeid(T); }
        TypeId getServiceInterfaceTypeId() const { return typeid(I); }

        IServiceHolder::Ptr create(ServiceProvider &serviceProvider) const
        {
            return create(serviceProvider, Indices{});
        };

      private:
        template <size_t... Index>
        typename ServiceOwner<I, T>::Ptr create(ServiceProvider &serviceProvider, std::index_sequence<Index...>) const
        {
            auto servicePtr =
                std::make_unique<T>(CtorArgProvider<typename ConstructorTraits::template Arg<Index>::Type>{}.getService(
                    serviceProvider)...);
            return std::make_unique<ServiceOwner<I, T>>(std::move(servicePtr));
        }
    };

} // namespace sb