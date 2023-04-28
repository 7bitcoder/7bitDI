#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/_Internal/IServiceHolder.hpp"

namespace sb
{
    class ServiceList
    {
      private:
        std::vector<IServiceHolder::Ptr> _serviceHolders;
        bool _sealed = false;

      public:
        ServiceList();

        ServiceList(ServiceList &&) = default;
        ServiceList(const ServiceList &) = delete;
        ServiceList &operator=(const ServiceList &) = delete;
        ServiceList &operator=(ServiceList &&) = default;

        void add(IServiceHolder::Ptr holder);

        void *get(TypeId typeId) const;

        void *at(size_t index = 0) const;

        bool empty() const;

        std::vector<void *> getAll() const;

        IServiceHolder *getHolder(TypeId typeId) const;

        IServiceHolder *getHolderAt(size_t index = 0) const;

        void reserve(size_t size);

        void seal();
        bool isSealed() const;

        bool contains(TypeId typeId) const;

        auto begin() const { return _serviceHolders.begin(); }
        auto end() const { return _serviceHolders.end(); }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceList.hpp"
#endif