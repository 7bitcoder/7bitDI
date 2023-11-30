#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/OptimalList.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    class EXPORT ServiceList
    {
      private:
        OptimalList<IServiceInstance::Ptr> _services;
        bool _sealed = false;

      public:
        explicit ServiceList(IServiceInstance::Ptr instance);

        ServiceList(const ServiceList &) = delete;
        ServiceList(ServiceList &&) = default;

        ServiceList &operator=(const ServiceList &) = delete;
        ServiceList &operator=(ServiceList &&) = default;

        [[nodiscard]] auto begin() const { return _services.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _services.getAsList().end(); }

        [[nodiscard]] auto rBegin() const { return _services.getAsList().rbegin(); }
        [[nodiscard]] auto rEnd() const { return _services.getAsList().rend(); }

        ServiceList &add(IServiceInstance::Ptr &&service);

        IServiceInstance::Ptr &last();

        IServiceInstance::Ptr &first();

        [[nodiscard]] std::vector<const IServiceInstance *> getAllServices() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] bool empty() const;

        void reserve(size_t size);

        void seal();

        [[nodiscard]] bool isSealed() const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceList.hpp"
#endif
