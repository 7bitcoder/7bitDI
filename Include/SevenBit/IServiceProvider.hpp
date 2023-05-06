#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/TypeId.hpp"

namespace sb
{
    struct IServiceProvider
    {
        using Ptr = std::unique_ptr<IServiceProvider>;

        virtual std::unique_ptr<IServiceProvider> createScope() = 0;

        virtual const IServiceInstance *tryGetInstance(TypeId serviceTypeId) = 0;

        template <class TService> TService *tryGetService()
        {
            if (auto instance = tryGetInstance(typeid(TService)))
            {
                return instance->getAs<TService>();
            }
            return nullptr;
        }

        virtual const IServiceInstance &getInstance(TypeId serviceTypeId) = 0;

        template <class TService> TService &getService() { return *getInstance(typeid(TService)).getAs<TService>(); }

        virtual std::vector<const IServiceInstance *> getInstances(TypeId serviceTypeId) = 0;

        template <class TService> std::vector<TService *> getServices()
        {
            auto instances = getInstances(typeid(TService));
            std::vector<TService *> result;
            result.reserve(instances.size());
            for (auto instance : instances)
            {
                if (instance)
                {
                    result.emplace_back(instance->getAs<TService>());
                }
            }
            return result;
        }

        virtual std::unique_ptr<IServiceInstance> tryCreateInstance(TypeId serviceTypeId) = 0;

        template <class TService> std::unique_ptr<TService> tryCreateService()
        {
            if (auto instance = tryCreateInstance(typeid(TService)))
            {
                return std::unique_ptr<TService>{instance->moveOutAs<TService>()};
            }
            return nullptr;
        }

        virtual std::unique_ptr<IServiceInstance> createInstance(TypeId serviceTypeId) = 0;

        template <class TService> std::unique_ptr<TService> createService()
        {
            return std::unique_ptr<TService>{createInstance(typeid(TService))->moveOutAs<TService>()};
        }

        virtual std::vector<std::unique_ptr<IServiceInstance>> createInstances(TypeId serviceTypeId) = 0;

        template <class TService> std::vector<std::unique_ptr<TService>> createServices()
        {
            auto instances = createInstances(typeid(TService));
            std::vector<std::unique_ptr<TService>> result;
            result.reserve(instances.size());
            for (auto &instance : instances)
            {
                if (instance)
                {
                    result.emplace_back(instance->moveOutAs<TService>());
                }
            }
            return result;
        }

        virtual ~IServiceProvider() = default;
    };
} // namespace sb
