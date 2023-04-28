#pragma once

#include <vector>

#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceCreators
    {
      private:
        std::vector<IServiceCreator::Ptr> _serviceCreators;

      public:
        ServiceCreators();

        ServiceCreators(ServiceCreators &&) = default;
        ServiceCreators(const ServiceCreators &) = delete;
        ServiceCreators &operator=(const ServiceCreators &) = delete;
        ServiceCreators &operator=(ServiceCreators &&) = default;

        ServiceLifeTime getServicesScope() const;

        void seal();

        auto begin() const { return _serviceCreators.begin(); }
        auto end() const { return _serviceCreators.end(); }

        TypeId getInterfaceTypeId() const;

        void add(IServiceCreator::Ptr creator);

        template <class T> size_t remove() { return remove(typeid(T)); }

        size_t remove(TypeId typeId);

        template <class T> bool contains() { return contains(typeid(T)); }

        bool contains(TypeId typeId);

        size_t size() const;

        IServiceCreator &getMainCreator() const;
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCreators.hpp"
#endif