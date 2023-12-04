#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/OneOrList.hpp"

namespace sb::di
{
    class EXPORT ServiceInstanceList
    {
      private:
        OneOrList<IServiceInstance::Ptr> _oneOrList;
        bool _sealed = false;

      public:
        explicit ServiceInstanceList(IServiceInstance::Ptr instance);
        explicit ServiceInstanceList(size_t size);

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        void add(IServiceInstance::Ptr &&service);

        OneOrList<IServiceInstance::Ptr> &getInnerList();
        [[nodiscard]] const OneOrList<IServiceInstance::Ptr> &getInnerList() const;

        IServiceInstance::Ptr &first();
        [[nodiscard]] const IServiceInstance::Ptr &first() const;

        IServiceInstance::Ptr &last();
        [[nodiscard]] const IServiceInstance::Ptr &last() const;

        IServiceInstance::Ptr &operator[](size_t index);
        const IServiceInstance::Ptr &operator[](size_t index) const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] bool empty() const;

        void reserve(size_t newCapacity);

        void shrink();

        void seal();

        [[nodiscard]] bool isSealed() const;

        template <class T> std::vector<T *> getServicesAs() const
        {
            std::vector<T *> result;
            if (auto instancePtr = _oneOrList.tryGetAsSingle())
            {
                auto &instance = *instancePtr;
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->getAs<T>());
                }
                return result;
            }
            result.reserve(size());
            for (auto &instance : _oneOrList.getAsList())
            {
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->getAs<T>());
                }
            }
            return result;
        }

        template <class T> std::vector<std::unique_ptr<T>> moveServicesAs() const
        {
            std::vector<std::unique_ptr<T>> result;
            if (auto instancePtr = _oneOrList.tryGetAsSingle())
            {
                auto &instance = *instancePtr;
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->moveOutAsUniquePtr<T>());
                }
                return result;
            }
            result.reserve(size());
            for (auto &instance : _oneOrList.getAsList())
            {
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->moveOutAsUniquePtr<T>());
                }
            }
            return result;
        }

        ~ServiceInstanceList() = default;
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceInstanceList.hpp"
#endif
