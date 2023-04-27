#pragma once

#include <cstddef>
#include <exception>
#include <forward_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SevenBit/_Internal/CtorArgProvider.hpp"
#include "SevenBit/_Internal/CtorReflection.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceScope.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"
#include "SevenBit/_Internal/Utils.hpp"

namespace sb
{
    class ServiceProvider;

    template <class T> T getService(ServiceProvider &provider);

    template <class I, class T> class ServiceConstructor final : public IServiceCreator
    {
      private:
        using ConstructorTraits = ConstrutorTraits<T>;
        using Indices = std::make_index_sequence<ConstructorTraits::ArgsSize>;
        ServiceScope _scope;

      public:
        using Ptr = std::unique_ptr<ServiceConstructor>;
        ServiceConstructor(const ServiceScope &scope) : _scope{scope}
        {
            static_assert(std::is_base_of_v<I, T>, "Type T must inherit from I");
        }

        const ServiceScope &getServiceScope() const { return _scope; };

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