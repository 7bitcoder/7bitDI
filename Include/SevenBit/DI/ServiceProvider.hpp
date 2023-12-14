#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"

namespace sb::di
{

    class ServiceProvider
    {
        IServiceInstanceProvider::Ptr _instanceProvider;

      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

        explicit ServiceProvider(IServiceInstanceProvider::Ptr instanceProvider)
        {
            _instanceProvider = std::move(instanceProvider);
            _instanceProvider->setServiceProvider(*this);
        }

        ServiceProvider(ServiceProvider &&parent) noexcept : ServiceProvider(std::move(parent._instanceProvider)) {}
        ServiceProvider(const ServiceProvider &parent) = delete;

        ServiceProvider &operator=(ServiceProvider &&parent) noexcept
        {
            _instanceProvider = std::move(parent._instanceProvider);
            _instanceProvider->setServiceProvider(*this);
            return *this;
        }
        ServiceProvider &operator=(const ServiceProvider &parent) = delete;

        [[nodiscard]] ServiceProvider createScope() const
        {
            return ServiceProvider{getInstanceProvider().createScope()};
        }

        [[nodiscard]] const IServiceInstanceProvider &getInstanceProvider() const
        {
            return *details::utils::Require::notNullAndGet(_instanceProvider.get());
        }
        IServiceInstanceProvider &getInstanceProvider()
        {
            return *details::utils::Require::notNullAndGet(_instanceProvider.get());
        }

        /**
         * @brief Returns service pointer, might be null
         * @details If service was not registered or was registered as transient, method returns null
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass * service = provider->tryGetService<TestClass>();
         * @endcode
         */
        template <class TService> TService *tryGetService()
        {
            if (const auto instance = getInstanceProvider().tryGetInstance(typeid(TService));
                details::utils::Check::instanceValidity(instance))
            {
                return instance->getAs<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Returns service reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @throws ServiceNotFoundException service was not found
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass & service = provider->getService<TestClass>();
         * @endcode
         */
        template <class TService> TService &getService()
        {
            auto &instance = getInstanceProvider().getInstance(typeid(TService));
            details::utils::Require::validInstance(&instance);
            return *instance.getAs<TService>();
        }

        /**
         * @brief Returns services
         * @details If instanceValidity was not registered or was registered as transient, method returns empty vector
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}
         *              .addScoped<ITestClass, TestClass1>()
         *              .addScoped<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<TService *> services = provider->getServices<ITestClass>();
         * @endcode
         */
        template <class TService> std::vector<TService *> getServices()
        {
            if (auto instances = getInstanceProvider().tryGetInstances(typeid(TService)))
            {
                return mapValidInstances(
                    *instances, [](const IServiceInstance::Ptr &instance) { return instance->getAs<TService>(); });
            }
            return {};
        }

        /**
         * @brief Creates instanceValidity unique pointner, might be null
         * @details If instanceValidity was not registered or was registered as scoped/transient, method returns null
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> instanceValidity = provider->tryCreateService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> tryCreateService()
        {
            if (const auto instance = getInstanceProvider().tryCreateInstance(typeid(TService));
                details::utils::Check::instanceValidity(instance))
            {
                return instance->moveOutAsUniquePtr<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Creates instanceValidity unique pointner, might throw exception
         * @details If instanceValidity was not registered or was registered as scoped/transient, method throws
         * exception
         * @throws ServiceNotFoundException instanceValidity was not found
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> instanceValidity = provider->createInstance<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> createService()
        {
            const auto instance = getInstanceProvider().createInstance(typeid(TService));
            details::utils::Require::validInstance(instance);
            return instance->moveOutAsUniquePtr<TService>();
        }

        template <class TService> TService createServiceInPlace()
        {
            const auto instance = getInstanceProvider().createInstanceInPlace(typeid(TService));
            details::utils::Require::validInstance(instance);
            return instance->moveOutAs<TService>();
        }

        /**
         * @brief Creates services
         * @details If instanceValidity was not registered or was registered as scoped/transient, method returns empty
         * vector
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}
         *              .addTransient<ITestClass, TestClass1>()
         *              .addTransient<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * d::vector<std::unique_ptr<ITestClass>> services = provider->createServices<ITestClass>();
         * @endcode
         */
        template <class TService> std::vector<std::unique_ptr<TService>> createServices()
        {
            if (auto instances = getInstanceProvider().tryCreateInstances(typeid(TService)))
            {
                return mapValidInstances(*instances, [](const IServiceInstance::Ptr &instance) {
                    return instance->moveOutAsUniquePtr<TService>();
                });
            }
            return {};
        }

      private:
        template <class OneOrList, class TFunc> auto mapValidInstances(OneOrList &instances, TFunc mapFunction)
        {
            std::vector<decltype(mapFunction(instances.first()))> result;
            if (auto instance = instances.tryGetAsSingle())
            {
                if (details::utils::Check::instanceValidity(*instance))
                {
                    result.reserve(1);
                    result.emplace_back(mapFunction(*instance));
                }
                return result;
            }
            result.reserve(instances.size());
            for (auto &instance : instances.getAsList())
            {
                if (details::utils::Check::instanceValidity(instance))
                {
                    result.emplace_back(mapFunction(instance));
                }
            }
            return result;
        }
    };
} // namespace sb::di
