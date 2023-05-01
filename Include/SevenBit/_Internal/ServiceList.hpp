#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"

namespace sb
{
    class ServiceList
    {
      private:
        std::vector<IServiceInstance::Ptr> _services;
        bool _sealed = false;

      public:
        ServiceList();

        ServiceList(ServiceList &&) = default;
        ServiceList(const ServiceList &) = delete;
        ServiceList &operator=(const ServiceList &) = delete;
        ServiceList &operator=(ServiceList &&) = default;

        ServiceList &add(IServiceInstance::Ptr service)
        {
            if (!service || !service->isValid())
            {
                throw ServiceHolderInvalidException{};
            }
            _services.push_back(std::move(service));
            return *this;
        }

        template <class TService> TService *get() { return (TService *)get(typeid(TService)); }

        void *get(TypeId typeId) const
        {
            if (auto instance = getInstance(typeId))
            {
                return instance->get();
            }
            return nullptr;
        }

        template <class TService> TService *at(size_t index = 0) const
        {
            if (auto instance = getInstanceAt(index))
            {
                return (TService *)instance->get();
            }
            return nullptr;
        }

        void *at(size_t index = 0) { return at<void>(index); }

        bool empty() const { return _services.empty(); }

        template <class TService> std::vector<TService *> getAll() const
        {
            std::vector<TService *> result;
            result.reserve(_services.size());
            for (auto &instance : _services)
            {
                result.push_back((TService *)instance->get());
            }
            return result;
        }

        std::vector<void *> getAll() const { return getAll<void>(); }

        IServiceInstance *getInstance(TypeId typeId) const
        {
            for (auto &holder : _services)
            {
                if (holder->getTypeId() == typeId)
                {
                    return holder.get();
                }
            }
            return nullptr;
        }

        IServiceInstance *getInstanceAt(size_t index = 0) const
        {
            if (index < _services.size())
            {
                return _services.at(index).get();
            }
            return nullptr;
        }

        void reserve(size_t size) { _services.reserve(size); }

        void seal()
        {
            _services.shrink_to_fit();
            _sealed = true;
        }

        bool isSealed() const { return _sealed; }

        bool contains(TypeId typeId) const { return getInstance(typeId); }

        auto begin() const { return _services.begin(); }
        auto end() const { return _services.end(); }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceList.hpp"
#endif