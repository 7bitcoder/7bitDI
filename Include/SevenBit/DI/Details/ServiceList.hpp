#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    class EXPORT ServiceList
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

        IServiceInstance::Ptr &last();

        IServiceInstance::Ptr &first();

        IServiceInstance::Ptr &at(size_t index = 0);

        std::vector<const IServiceInstance *> getAllServices() const;

        bool empty() const;

        void reserve(size_t size);

        void seal();

        bool isSealed() const;
    };
} // namespace sb::di::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceList.hpp"
#endif