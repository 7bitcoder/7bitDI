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

    template <class I, class T, class FactoryFcn> class ServiceFactory final : public IServiceCreator
    {
      private:
        FactoryFcn _factoryFunction;
        ServiceScope _scope;

      public:
        ServiceFactory(const ServiceScope &scope, FactoryFcn factoryFunction)
            : _factoryFunction{std::move(factoryFunction)}, _scope{scope}
        {
            static_assert(std::is_base_of_v<I, T>, "Type T must inherit from I");
            // static_assert(std::is_same_v<std::unique_ptr<T>, decltype(std::declval<FactoryFcn>())>,
            //               "Factory function must return unique pointner");
        }

        const ServiceScope &getServiceScope() const { return _scope; }

        TypeId getServiceTypeId() const { return typeid(T); }
        TypeId getServiceInterfaceTypeId() const { return typeid(I); }

        IServiceHolder::Ptr create(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ServiceOwner<I, T>>(_factoryFunction(serviceProvider));
        }
    };

} // namespace sb