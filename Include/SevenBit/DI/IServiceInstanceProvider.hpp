#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{

    struct IServiceInstanceProvider
    {
        using Ptr = std::unique_ptr<IServiceInstanceProvider>;

        /**
         * @brief Create a scoped service provider
         * @details Scoped service provider creates/holds its own scoped services
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         * auto scoped = provider->createScope();
         *
         * &scoped->getService<TestClass>() != &provider->getService<TestClass>(); // True
         * @endcode
         */
        virtual IServiceInstanceProvider::Ptr createScope() = 0;

        /**
         * @brief Returns service instance pointner, might be null
         * @details If service was not registered or was registered as transient, method returns null
         * @attention It is advised to use tryGetService<T> method istead
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * IServiceInstance * instance = provider->tryGetInstance(typeid(TestClass));
         * @endcode
         */
        virtual const IServiceInstance *get(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service instances
         * @details If service was not registered or was registered as transient, method returns empty vector
         * @attention It is advised to use getServices<T> method istead
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}
         *              .addScoped<ITestClass, TestClass1>()
         *              .addScoped<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<const IServiceInstance *> instances = provider->getInstances(typeid(ITestClass));
         * @endcode
         */
        virtual std::vector<const IServiceInstance *> getAll(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instance unique pointner, might be null
         * @details If service was not registered or was registered as scoped/transient, method returns null
         * @attention It is advised to use tryCreateService<T> method istead
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instance = provider->tryCreateInstance(typeid(TestClass));
         * @endcode
         */
        virtual std::unique_ptr<IServiceInstance> create(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instances
         * @details If service was not registered or was registered as scoped/transient, method returns empty vector
         * @attention It is advised to use createServices<T> method istead
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}
         *              .addTransient<ITestClass, TestClass1>()
         *              .addTransient<ITestClass, TestClass2>()
         *              .buildServiceProvider();
         *
         * std::vector<std::unique_ptr<IServiceInstance>> instances = provider->createInstances(typeid(ITestClass));
         * @endcode
         */
        virtual std::vector<std::unique_ptr<IServiceInstance>> createAll(TypeId serviceTypeId) = 0;

        virtual ~IServiceInstanceProvider() = default;
    };
} // namespace sb::di
