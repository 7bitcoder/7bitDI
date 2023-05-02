#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct IServiceProvider
    {
        using Ptr = std::unique_ptr<IServiceProvider>;

        virtual Ptr createScope() = 0;

        virtual void *getService(TypeId serviceTypeId) = 0;

        template <class TService> TService *getService()
        {
            return static_cast<TService *>(getService(typeid(TService)));
        }

        virtual void *getRequiredService(TypeId serviceTypeId) = 0;

        template <class TService> TService &getRequiredService()
        {
            return *static_cast<TService *>(getRequiredService(typeid(TService)));
        }

        virtual std::vector<void *> getServices(TypeId serviceTypeId) = 0;

        template <class TService> std::vector<TService *> getServices()
        {
            auto services = getServices(typeid(TService));
            std::vector<TService *> result;
            result.reserve(services.size());
            for (auto ptr : services)
            {
                result.emplace_back(static_cast<TService *>(ptr));
            }
            return result;
        }

        virtual void *createService(TypeId serviceTypeId) = 0;

        template <class TService> std::unique_ptr<TService> createService()
        {
            return std::unique_ptr<TService>{static_cast<TService *>(createService(typeid(TService)))};
        }

        virtual void *createRequiredService(TypeId serviceTypeId) = 0;

        template <class TService> std::unique_ptr<TService> createRequiredService()
        {
            return std::unique_ptr<TService>{static_cast<TService *>(createRequiredService(typeid(TService)))};
        }

        virtual std::vector<void *> createServices(TypeId serviceTypeId) = 0;

        template <class TService> std::vector<std::unique_ptr<TService>> createServices()
        {
            auto services = createServices(typeid(TService));
            std::vector<std::unique_ptr<TService>> result;
            result.reserve(services.size());
            for (auto ptr : services)
            {
                result.emplace_back(static_cast<TService *>(ptr));
            }
            return result;
        }

        virtual ~IServiceProvider() = default;
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif
