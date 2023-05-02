#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"

namespace sb::internal
{
    class ServiceList
    {
      private:
        std::vector<IServiceInstance::Ptr> _services;
        bool _sealed = false;

      public:
        ServiceList() = default;

        ServiceList(ServiceList &&) = default;
        ServiceList(const ServiceList &) = delete;
        ServiceList &operator=(const ServiceList &) = delete;
        ServiceList &operator=(ServiceList &&) = default;

        auto begin() const { return _services.begin(); }
        auto end() const { return _services.end(); }

        auto rBegin() const { return _services.rbegin(); }
        auto rEnd() const { return _services.rend(); }

        ServiceList &add(IServiceInstance::Ptr service);

        void *get(TypeId typeId) const;

        void *first();

        void *at(size_t index = 0);

        bool empty() const;

        std::vector<void *> getAll() const;

        IServiceInstance *getInstance(TypeId typeId) const;

        IServiceInstance *getInstanceAt(size_t index = 0) const;

        void reserve(size_t size);

        void seal();

        bool isSealed() const;

        bool contains(TypeId typeId) const;
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceList.hpp"
#endif