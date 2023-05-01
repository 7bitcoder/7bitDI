#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceDescriptorList
    {
      private:
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        ServiceDescriptorList();

        ServiceDescriptorList(ServiceDescriptorList &&) = default;
        ServiceDescriptorList(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(ServiceDescriptorList &&) = default;

        const ServiceLifeTime &getLifeTime() const { return last().getLifeTime(); }

        TypeId getServiceTypeId() const { return last().getServiceTypeId(); }

        void seal() { _serviceDescriptors.shrink_to_fit(); }

        auto begin() const { return _serviceDescriptors.begin(); }
        auto end() const { return _serviceDescriptors.end(); }

        void add(ServiceDescriptor descriptor)
        {
            checkIfRegistered(descriptor);
            checkLifeTime(descriptor);
            _serviceDescriptors.emplace_back(std::move(descriptor));
        }

        template <class T> size_t remove() { return remove(typeid(T)); }

        size_t remove(TypeId typeId)
        {
            return std::erase_if(_serviceDescriptors, [&](ServiceDescriptor &descriptor) {
                return descriptor.getImplementationTypeId() == typeId;
            });
        }

        template <class T> bool contains() { return contains(typeid(T)); }

        bool contains(TypeId typeId)
        {
            return std::find_if(begin(), end(), [&](const ServiceDescriptor &descriptor) {
                       return descriptor.getImplementationTypeId() == typeId;
                   }) != end();
        }

        size_t size() const { return _serviceDescriptors.size(); }

        const ServiceDescriptor &last() const { return _serviceDescriptors.back(); }

        ServiceDescriptor &last() { return _serviceDescriptors.back(); }

      private:
        void checkIfRegistered(ServiceDescriptor &descriptor)
        {
            if (contains(descriptor.getImplementationTypeId()))
            {
                // todo throw
            }
        }

        void checkLifeTime(ServiceDescriptor &descriptor)
        {
            if (!_serviceDescriptors.empty() && last().getLifeTime() != descriptor.getLifeTime())
            {
                // todo throw
            }
        }
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCreators.hpp"
#endif