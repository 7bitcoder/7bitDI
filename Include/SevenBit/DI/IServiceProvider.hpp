#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{

    struct IServiceProvider
    {
        using Ptr = std::unique_ptr<IServiceProvider>;

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
        virtual IServiceProvider::Ptr createScope() = 0;

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
        virtual const IServiceInstance *tryGetInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service pointner, might be null
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
            if (auto instance = tryGetInstance(typeid(TService)))
            {
                return instance->getAs<TService>();
            }
            return nullptr;
        }

        /**
         * @brief Returns service instance reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @attention It is advised to use getService<T> method istead
         * @throws ServiceNotFoundException service was not found
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addScoped<TestClass>().buildServiceProvider();
         *
         * const IServiceInstance & instance = provider->getInstance(typeid(TestClass));
         * @endcode
         */
        virtual const IServiceInstance &getInstance(TypeId serviceTypeId) = 0;

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
        template <class TService> TService &getService() { return *getInstance(typeid(TService)).getAs<TService>(); }

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
        virtual std::vector<const IServiceInstance *> getInstances(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns services
         * @details If service was not registered or was registered as transient, method returns empty vector
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
        virtual std::unique_ptr<IServiceInstance> tryCreateInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service unique pointner, might be null
         * @details If service was not registered or was registered as scoped/transient, method returns null
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider->tryCreateService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> tryCreateService()
        {
            if (auto instance = tryCreateInstance(typeid(TService)))
            {
                return std::unique_ptr<TService>{instance->moveOutAs<TService>()};
            }
            return nullptr;
        }

        /**
         * @brief Creates service instance unique pointner, might throw exception
         * @details If service was not registered or was registered as scoped/transient, method throws exception
         * @attention It is advised to use createService<T> method istead
         * @throws ServiceNotFoundException service was not found
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<IServiceInstance> instance = provider->createInstance(typeid(TestClass));
         * @endcode
         */
        virtual std::unique_ptr<IServiceInstance> createInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service unique pointner, might throw exception
         * @details If service was not registered or was registered as scoped/transient, method throws exception
         * @throws ServiceNotFoundException service was not found
         *
         * Example:
         * @code {.cpp}
         * auto provider = ServiceCollection{}.addTransient<TestClass>().buildServiceProvider();
         *
         * std::unique_ptr<TestClass> service = provider->createService<TestClass>();
         * @endcode
         */
        template <class TService> std::unique_ptr<TService> createService()
        {
            return std::unique_ptr<TService>{createInstance(typeid(TService))->moveOutAs<TService>()};
        }

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
        virtual std::vector<std::unique_ptr<IServiceInstance>> createInstances(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates services
         * @details If service was not registered or was registered as scoped/transient, method returns empty vector
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
} // namespace sb::di
