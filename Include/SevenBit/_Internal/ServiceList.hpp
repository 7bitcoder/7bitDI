#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceScope.hpp"

namespace sb
{
    class ServiceList
    {
      private:
        std::vector<IServiceHolder::Ptr> _serviceHolders;
        bool _sealed = false;

      public:
        ServiceList() { _serviceHolders.reserve(1); }

        ServiceList(ServiceList &&) = default;
        ServiceList(const ServiceList &) = delete;
        ServiceList &operator=(const ServiceList &) = delete;
        ServiceList &operator=(ServiceList &&) = default;

        void add(IServiceHolder::Ptr holder)
        {
            if (!holder || !holder->isValid())
            {
                throw ServiceHolderInvalidException{};
            }
            _serviceHolders.push_back(std::move(holder));
        }

        void *get(TypeId typeId) const
        {
            if (auto serviceHolderPtr = getHolder(typeId))
            {
                return serviceHolderPtr->getService();
            }
            return nullptr;
        }

        void *at(size_t index = 0) const
        {
            if (auto serviceHolderPtr = getHolderAt(index))
            {
                return serviceHolderPtr->getService();
            }
            return nullptr;
        }

        bool empty() const { return _serviceHolders.empty(); }

        std::vector<void *> getAll() const
        {
            std::vector<void *> result;
            result.reserve(_serviceHolders.size());
            for (auto &holder : _serviceHolders)
            {
                result.push_back(holder->getService());
            }
            return result;
        }

        IServiceHolder *getHolder(TypeId typeId) const
        {
            for (auto &holder : _serviceHolders)
            {
                if (holder->getServiceTypeId() == typeId)
                {
                    return holder.get();
                }
            }
            return nullptr;
        }

        IServiceHolder *getHolderAt(size_t index = 0) const
        {
            if (index < _serviceHolders.size())
            {
                return _serviceHolders.at(index).get();
            }
            return nullptr;
        }

        void reserve(size_t size) { _serviceHolders.reserve(size); }

        void seal()
        {
            _serviceHolders.shrink_to_fit();
            _sealed = true;
        }

        bool isSealed() const { return _sealed; }

        bool contains(TypeId typeId) const { return getHolder(typeId); }

        auto begin() const { return _serviceHolders.cbegin(); }
        auto end() const { return _serviceHolders.cend(); }
    };
} // namespace sb