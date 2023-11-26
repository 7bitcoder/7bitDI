#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstanceProvider.hpp"

namespace sb::di
{

    struct ServiceProvider : public IServiceInstanceProvider
    {
        using Ptr = std::unique_ptr<ServiceProvider>;

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
        virtual ServiceProvider::Ptr createScope() = 0;

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
            auto instance = getInstance(typeid(TService));
            return instance && instance->isValid() ? instance->getAs<TService>() : nullptr;
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
            auto service = tryGetService<TService>();
            return service ? *service
                           : throw ServiceNotFoundException{
                                 typeid(TService), "Service was not registered or was registered as transient service"};
        }

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
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->getAs<TService>());
                }
            }
            return result;
        }

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
            auto instance = createInstance(typeid(TService));
            return instance && instance->isValid() ? std::unique_ptr<TService>{instance->moveOutAs<TService>()}
                                                   : nullptr;
        }

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
            auto service = tryCreateService<TService>();
            return service ? std::move(service)
                           : throw ServiceNotFoundException{
                                 typeid(TService),
                                 "Service was not registered or was registered as singleton/scoped service"};
        }

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
                if (instance && instance->isValid())
                {
                    result.emplace_back(instance->moveOutAs<TService>());
                }
            }
            return result;
        }
    };
} // namespace sb::di
