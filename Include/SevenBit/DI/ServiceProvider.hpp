#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/IServiceInstanceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di
{
    class EXPORT ServiceProvider
    {
        IServiceInstanceProvider::Ptr _instanceProvider;

      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

        /**
         * @brief Constructs service provider with specified instance provider
         */
        explicit ServiceProvider(IServiceInstanceProvider::Ptr instanceProvider);

        ServiceProvider(const ServiceProvider &parent) = delete;
        ServiceProvider(ServiceProvider &&) = delete;

        ServiceProvider &operator=(const ServiceProvider &parent) = delete;
        ServiceProvider &operator=(ServiceProvider &&parent) = delete;

        /**
         * @brief Get service provider options
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * auto& usedOptions = provider.getOptions();
         * @endcode
         */
        [[nodiscard]] const ServiceProviderOptions &getOptions() const;

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
        [[nodiscard]] ServiceProvider createScope() const;

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
        [[nodiscard]] Ptr createScopeAsPtr() const;

        /**
         * @brief Returns inner service instance provider
         * @details If service instance provider is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        [[nodiscard]] const IServiceInstanceProvider &getInstanceProvider() const;

        /**
         * @brief Returns inner service instance provider
         * @details If service instance provider is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        IServiceInstanceProvider &getInstanceProvider();

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
            details::utils::Require::validInstance(instance);
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
            if (auto instances = getInstanceProvider().tryGetInstances(typeid(TService)))
            {
                return instances->map([&](const ServiceInstance &instance) {
                    details::utils::Require::validInstance(instance);
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
            if (auto instance = getInstanceProvider().tryCreateInstance(typeid(TService));
                details::utils::Check::instanceValidity(&instance))
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
            details::utils::Require::validInstance(instance);
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
            details::utils::Require::validInstance(instance);
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
            if (auto instances = getInstanceProvider().tryCreateInstances(typeid(TService)))
            {
                return instances->map([&](ServiceInstance &instance) {
                    details::utils::Require::validInstance(instance);
                    return instance.moveOutAsUniquePtr<TService>();
                });
            }
            return {};
        }
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceProvider.hpp"
#endif
