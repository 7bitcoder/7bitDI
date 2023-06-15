#pragma once

#include <type_traits>

#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

namespace sb::di
{
    /**
     * @brief This is helper factory class for constructing ServiceDescriptor
     */
    class ServiceDescriber
    {
      public:
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeSingleton<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeSingleton<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeSingleton()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::singleton());
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeScoped<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeScoped<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeScoped()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::scoped());
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeTransient<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeTransient<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeTransient()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::transient());
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - given lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describe<TestClass>(ServiceLifeTime::scoped());
         * ServiceDescriptor descriptor2 =
         *          ServiceDescriber::describe<BaseClass, ImplementationClass>(ServiceLifeTime::transient());
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime)
        {
            details::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<details::ServiceCtorFactory<TImplementation>>();
            return {typeid(TService), lifetime, std::move(factory)};
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external service pointner
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         *
         * Example:
         * @code {.cpp}
         * TestClass test;
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeSingleton<TestClass>(test);
         *
         * ImplementationClass implementation;
         * ServiceDescriptor descriptor =
         *              ServiceDescriber::describeSingleton<BaseClass, ImplementationClass>(implementation);
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeSingleton(TImplementation &service)
        {
            return describeSingleton<TService, TImplementation>(&service);
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external service pointner
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         *
         * Example:
         * @code {.cpp}
         * TestClass test;
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeSingleton<TestClass>(&test);
         *
         * ImplementationClass implementation;
         * ServiceDescriptor descriptor =
         *              ServiceDescriber::describeSingleton<BaseClass, ImplementationClass>(&implementation);
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeSingleton(TImplementation *service)
        {
            details::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<details::ExternalServiceFactory<TImplementation>>(service);
            return {typeid(TService), ServiceLifeTime::singleton(), std::move(factory)};
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::singleton(), std::move(factory));
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::scoped(), std::move(factory));
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::transient(), std::move(factory));
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must inherit from TService and must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); }, ServiceLifeTime::scoped());
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFrom(ServiceLifeTime lifetime, FactoryFcn &&factoryFcn)
        {
            using FactoryType = details::ServiceFcnFactory<FactoryFcn>;
            auto factory = std::make_unique<FactoryType>(std::move(factoryFcn));

            using Service =
                typename std::conditional<std::is_void_v<TService>, typename FactoryType::ServiceType, TService>::type;
            return {typeid(Service), lifetime, std::move(factory)};
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::singleton(), std::move(factory));
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::scoped(), std::move(factory));
        }
        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::transient(), std::move(factory));
        }

        /**
         * @brief Creates ServiceDescriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this sheme: (const ServiceDescriptor&) ->
         * std::unique_ptr<TImplementation> (argument is optional), functor must be copyable and movable, implementation
         * type must have one constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom(
         *       []() { return std::make_unique<TestClass>(); }, ServiceLifeTime::scoped());
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeFrom(ServiceLifeTime lifetime, FactoryFcn &&factoryFcn)
        {
            return describeFrom<void, FactoryFcn>(lifetime, std::move(factoryFcn));
        }

        template <class TService, class TImplementation = TService> static ServiceDescriptor describeAlias()
        {
            using FactoryType = details::ServiceFcnFactory<FactoryFcn>;
            auto factory = std::make_unique<FactoryType>(std::move(factoryFcn));

            using Service =
                typename std::conditional<std::is_void_v<TService>, typename FactoryType::ServiceType, TService>::type;
            return {typeid(Service), lifetime, std::move(factory)};
        }
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#endif