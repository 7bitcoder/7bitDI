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
#include "SevenBit/_Internal/Utils.hpp"

namespace sb
{
    class ServiceProvider;

    class ServiceCreators
    {
      private:
        std::vector<IServiceCreator::Ptr> _serviceCreators;

      public:
        ServiceCreators() { _serviceCreators.reserve(1); }

        ServiceCreators(ServiceCreators &&) = default;
        ServiceCreators(const ServiceCreators &) = delete;
        ServiceCreators &operator=(const ServiceCreators &) = delete;
        ServiceCreators &operator=(ServiceCreators &&) = default;

        ServiceScope getServicesScope() const { return getMainCreator().getServiceScope(); }

        TypeId getInterfaceTypeId() const { return getMainCreator().getServiceInterfaceTypeId(); }

        void add(IServiceCreator::Ptr creator)
        {
            if (!creator)
            {
                throw ServiceCreatorInvalidException{};
            }
            _serviceCreators.emplace_back(std::move(creator));
        }

        template <class T> size_t remove()
        {
            TypeId type = typeid(T);
            return std::erase_if(_serviceCreators, [=](auto &creator) { return creator.getServiceTypeId() == type; });
        }

        template <class T> bool contains()
        {
            TypeId type = typeid(T);
            for (auto &creator : _serviceCreators)
            {
                if (creator->getServiceTypeId() == type)
                {
                    return true;
                }
            }
            return false;
        }

        void seal() { _serviceCreators.shrink_to_fit(); }

        auto begin() const { return _serviceCreators.crbegin(); }
        auto end() const { return _serviceCreators.crend(); }

        size_t size() const { return _serviceCreators.size(); }

        IServiceCreator &getMainCreator() const { return *_serviceCreators.back(); }
    };

} // namespace sb