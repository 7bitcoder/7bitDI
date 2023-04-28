#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceProvider;

    template <class I, class T, class FactoryFcn> class ServiceFactory final : public IServiceCreator
    {
      private:
        FactoryFcn _factoryFunction;
        ServiceLifeTime _scope;

      public:
        ServiceFactory(const ServiceLifeTime &scope, FactoryFcn factoryFunction)
            : _factoryFunction{std::move(factoryFunction)}, _scope{scope}
        {
            static_assert(std::is_base_of_v<I, T>, "Type T must inherit from I");
            // static_assert(std::is_same_v<std::unique_ptr<T>, decltype(std::declval<FactoryFcn>())>,
            //               "Factory function must return unique pointner");
        }

        const ServiceLifeTime &getServiceScope() const { return _scope; }

        TypeId getServiceTypeId() const { return typeid(T); }
        TypeId getServiceInterfaceTypeId() const { return typeid(I); }

        IServiceHolder::Ptr create(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ServiceOwner<I, T>>(_factoryFunction(serviceProvider));
        }
    };

} // namespace sb