#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"

namespace sb::di
{
    class ServiceProvider
    {
        IServiceInstanceProvider::Ptr _instanceProvider;

      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

        /**
         * @brief Constructs service provider with specified instance provider
         */
        explicit ServiceProvider(IServiceInstanceProvider::Ptr instanceProvider)
            : _instanceProvider(std::move(instanceProvider))
        {
            details::Require::notNull(_instanceProvider);
            getInstanceProvider().init(*this);
        }

        ServiceProvider(const ServiceProvider &parent) = delete;
        ServiceProvider(ServiceProvider &&) = delete;

        ServiceProvider &operator=(const ServiceProvider &parent) = delete;
        ServiceProvider &operator=(ServiceProvider &&parent) = delete;

        /**
         * @brief Returns inner service instance provider
         * @details If service instance provider is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        [[nodiscard]] const IServiceInstanceProvider &getInstanceProvider() const { return *_instanceProvider; }

        /**
         * @brief Returns inner service instance provider
         * @details If service instance provider is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        IServiceInstanceProvider &getInstanceProvider() { return *_instanceProvider; }

        /**
         * @brief Create a scoped service provider
         * @details Scoped service provider creates/holds its own scoped services
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         * auto scoped = provider.createScope();
         *
         * &scoped.getService<TestClass>() != &provide.getService<TestClass>(); // True
         * @endcode
         */
        [[nodiscard]] ServiceProvider createScope() const
        {
            return ServiceProvider{getInstanceProvider().createScope()};
        }

        /**
         * @brief Create a scoped service provider as unique_ptr
         * @details Scoped service provider creates/holds its own scoped services
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProviderAsPtr();
         * auto scoped = provider->createScopeAsPtr();
         *
         * &scoped->getService<TestClass>() != &provider->getService<TestClass>(); // True
         * @endcode
         */
        [[nodiscard]] Ptr createScopeAsPtr() const
        {
            return std::make_unique<ServiceProvider>(getInstanceProvider().createScope());
        }

        /**
         * @brief Returns service pointer, might be null
         * @details If service was not registered or was registered as transient, method returns null
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass* service = provider.tryGetService<TestClass>();
         * @endcode
         */
        template <class TService> TService *tryGetService()
        {
            if (const auto instancePtr = getInstanceProvider().tryGetInstance(typeid(TService));
                instancePtr && *instancePtr)
            {
                return instancePtr->getAs<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Returns service pointer, might be null
         * @details If service was not registered or was registered as transient, method returns null
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass* service = provider.tryGetService<TestClass>();
         * @endcode
         */
        template <class TService> TService *tryGetKeyedService(const std::string_view serviceKey)
        {
            if (const auto instancePtr = getInstanceProvider().tryGetKeyedInstance(typeid(TService), serviceKey);
                instancePtr && *instancePtr)
            {
                return instancePtr->getAs<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Returns service reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @throws sb::di::ServiceNotFoundException
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass& service = provider.getService<TestClass>();
         * @endcode
         */
        template <class TService> TService &getService()
        {
            auto &instance = getInstanceProvider().getInstance(typeid(TService));
            details::RequireInstance::valid(instance);
            return *instance.getAs<TService>();
        }

        /**
         * @brief Returns service reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @throws sb::di::ServiceNotFoundException
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * TestClass& service = provider.getService<TestClass>();
         * @endcode
         */
        template <class TService> TService &getKeyedService(const std::string_view serviceKey)
        {
            auto &instance = getInstanceProvider().getKeyedInstance(typeid(TService), serviceKey);
            details::RequireInstance::valid(instance);
            return *instance.getAs<TService>();
        }

        /**
         * @brief Returns services
         * @details If service was not registered or was registered as transient, method returns empty vector
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addScoped<ITestClass, TestClass1>()
         *              .addScoped<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<ITestClass *> services = provider.getServices<ITestClass>();
         * @endcode
         */
        template <class TService> std::vector<TService *> getServices()
        {
            if (auto instancesPtr = getInstanceProvider().tryGetInstances(typeid(TService)))
            {
                return instancesPtr->map([](const ServiceInstance &instance) {
                    details::RequireInstance::valid(instance);
                    return instance.getAs<TService>();
                });
            }
            return {};
        }

        /**
         * @brief Returns services
         * @details If service was not registered or was registered as transient, method returns empty vector
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addScoped<ITestClass, TestClass1>()
         *              .addScoped<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<ITestClass *> services = provider.getServices<ITestClass>();
         * @endcode
         */
        template <class TService> std::vector<TService *> getKeyedServices(const std::string_view serviceKey)
        {
            if (auto instancesPtr = getInstanceProvider().tryGetKeyedInstances(typeid(TService), serviceKey))
            {
                return instancesPtr->map([](const ServiceInstance &instance) {
                    details::RequireInstance::valid(instance);
                    return instance.getAs<TService>();
                });
            }
            return {};
        }

        /**
         * @brief Creates service unique pointer, might be null
         * @details If service was not registered or was registered as scoped/singleton, method returns null
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider.tryCreateService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> tryCreateService()
        {
            if (auto instance = getInstanceProvider().tryCreateInstance(typeid(TService)))
            {
                return instance.moveOutAsUniquePtr<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Creates service unique pointer, might be null
         * @details If service was not registered or was registered as scoped/singleton, method returns null
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider.tryCreateService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> tryCreateKeyedService(const std::string_view serviceKey)
        {
            if (auto instance = getInstanceProvider().tryCreateKeyedInstance(typeid(TService), serviceKey))
            {
                return instance.moveOutAsUniquePtr<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Creates service unique pointer, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider.createService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> createService()
        {
            auto instance = getInstanceProvider().createInstance(typeid(TService));
            details::RequireInstance::valid(instance);
            return instance.moveOutAsUniquePtr<TService>();
        }

        /**
         * @brief Creates service unique pointer, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider.createService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> createKeyedService(const std::string_view serviceKey)
        {
            auto instance = getInstanceProvider().createKeyedInstance(typeid(TService), serviceKey);
            details::RequireInstance::valid(instance);
            return instance.moveOutAsUniquePtr<TService>();
        }

        /**
         * @brief Creates service in place, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * TestClass service = provider.createServiceInPlace<TestClass>();
         * @endcode
         */
        template <class TService> TService createServiceInPlace()
        {
            auto instance = getInstanceProvider().createInstanceInPlace(typeid(TService));
            details::RequireInstance::valid(instance);
            if constexpr (std::is_move_constructible_v<TService>)
            {
                return instance.moveOutAs<TService>();
            }
            else
            {
                return instance.copyAs<TService>();
            }
        }

        /**
         * @brief Creates service in place, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * TestClass service = provider.createServiceInPlace<TestClass>();
         * @endcode
         */
        template <class TService> TService createKeyedServiceInPlace(const std::string_view serviceKey)
        {
            auto instance = getInstanceProvider().createKeyedInstanceInPlace(typeid(TService), serviceKey);
            details::RequireInstance::valid(instance);
            if constexpr (std::is_move_constructible_v<TService>)
            {
                return instance.moveOutAs<TService>();
            }
            else
            {
                return instance.copyAs<TService>();
            }
        }

        /**
         * @brief Creates services
         * @details If service was not registered or was registered as scoped/singleton, method returns empty vector
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addTransient<ITestClass, TestClass1>()
         *              .addTransient<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<std::unique_ptr<ITestClass>> services = provider.createServices<ITestClass>();
         * @endcode
         */
        template <class TService> std::vector<std::unique_ptr<TService>> createServices()
        {
            if (auto optInstances = getInstanceProvider().tryCreateInstances(typeid(TService)))
            {
                return optInstances->map([&](ServiceInstance &instance) {
                    details::RequireInstance::valid(instance);
                    return instance.moveOutAsUniquePtr<TService>();
                });
            }
            return {};
        }

        /**
         * @brief Creates services
         * @details If service was not registered or was registered as scoped/singleton, method returns empty vector
         * @param serviceKey service key can be empty to get default service
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addTransient<ITestClass, TestClass1>()
         *              .addTransient<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<std::unique_ptr<ITestClass>> services = provider.createServices<ITestClass>();
         * @endcode
         */
        template <class TService>
        std::vector<std::unique_ptr<TService>> createKeyedServices(const std::string_view serviceKey)
        {
            if (auto optInstances = getInstanceProvider().tryCreateKeyedInstances(typeid(TService), serviceKey))
            {
                return optInstances->map([&](ServiceInstance &instance) {
                    details::RequireInstance::valid(instance);
                    return instance.moveOutAsUniquePtr<TService>();
                });
            }
            return {};
        }
    };
} // namespace sb::di
