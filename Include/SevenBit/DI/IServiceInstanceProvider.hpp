#pragma once

#include <memory>
#include <optional>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    class ServiceProvider;

    struct IServiceInstanceProvider
    {
        using Ptr = std::unique_ptr<IServiceInstanceProvider>;

        virtual void setServiceProvider(ServiceProvider &serviceProvider) = 0;

        /**
         * @brief Get service provider options
         * @details Scoped service provider creates/holds its own scoped services
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         * auto scoped = provider->createScope();
         *
         * auto& usedOptions = &provider->getOptions(); // True
         * @endcode
         */
        [[nodiscard]] virtual const ServiceProviderOptions &getOptions() const = 0;

        /**
         * @brief Create a scoped service provider
         * @details Scoped service provider creates/holds its own scoped services
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         * auto scoped = provider->createScope();
         *
         * &scoped->getService<TestClass>() != &provider->getService<TestClass>(); // True
         * @endcode
         */
        [[nodiscard]] virtual Ptr createScope() const = 0;

        /**
         * @brief Returns service service reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @attention It is advised to use getService<T> method istead
         * @throws ServiceNotFoundException service was not found
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * const IServiceInstance & service = provider->getInstance(typeid(TestClass));
         * @endcode
         */
        virtual const IServiceInstance &getInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service service pointner, might be null
         * @details If service was not registered or was registered as transient, method returns null
         * @attention It is advised to use tryGetService<T> method istead
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * IServiceInstance * service = provider->tryGetInstance(typeid(TestClass));
         * @endcode
         */
        virtual const IServiceInstance *tryGetInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service instances
         * @details If instanceValidity was not registered or was registered as transient, method returns empty vector
         * @attention It is advised to use getServices<T> method istead
         * @example
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addScoped<ITestClass, TestClass1>()
         *              .addScoped<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<const IServiceInstance *> instances = provider->getInstances(typeid(ITestClass));
         * @endcode
         */
        virtual const OneOrList<IServiceInstance::Ptr> *tryGetInstances(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates instanceValidity instanceValidity unique pointner, might throw exception
         * @details If instanceValidity was not registered or was registered as scoped/transient, method throws
         * exception
         * @attention It is advised to use createInstance<T> method istead
         * @throws ServiceNotFoundException instanceValidity was not found
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instanceValidity = provider->createInstance(typeid(TestClass));
         * @endcode
         */
        virtual IServiceInstance::Ptr createInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates instanceValidity instanceValidity unique pointner, might be null
         * @details If instanceValidity was not registered or was registered as scoped/transient, method returns null
         * @attention It is advised to use tryCreateService<T> method istead
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instanceValidity = provider->tryCreateInstance(typeid(TestClass));
         * @endcode
         */
        virtual IServiceInstance::Ptr tryCreateInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates instanceValidity instanceValidity unique pointner, might throw exception
         * @details If instanceValidity was not registered or was registered as scoped/transient, method throws
         * exception
         * @attention It is advised to use createInstance<T> method istead
         * @throws ServiceNotFoundException instanceValidity was not found
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instanceValidity = provider->createInstance(typeid(TestClass));
         * @endcode
         */
        virtual IServiceInstance::Ptr createInstanceInPlace(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates instanceValidity instanceValidity unique pointner, might be null
         * @details If instanceValidity was not registered or was registered as scoped/transient, method returns null
         * @attention It is advised to use tryCreateService<T> method istead
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instanceValidity = provider->tryCreateInstance(typeid(TestClass));
         * @endcode
         */
        virtual IServiceInstance::Ptr tryCreateInstanceInPlace(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates instanceValidity instances
         * @details If instanceValidity was not registered or was registered as scoped/transient, method returns empty
         * vector
         * @attention It is advised to use createServices<T> method istead
         *
         * Example:
         * @code{.cpp}
         * auto provider = ServiceCollection{}
         *              .addTransient<ITestClass, TestClass1>()
         *              .addTransient<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<std::unique_ptr<IServiceInstance>> instances = provider->createInstances(typeid(ITestClass));
         * @endcode
         */
        virtual std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateInstances(TypeId serviceTypeId) = 0;

        virtual ~IServiceInstanceProvider() = default;
    };
} // namespace sb::di
