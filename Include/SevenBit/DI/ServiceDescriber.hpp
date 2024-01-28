#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Factories/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/Factories/UniquePtrServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/Factories/VoidServiceFactory.hpp"
#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/Details/Utils/Assert.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTimes.hpp"

namespace sb::di
{
    class ServiceDescriber
    {
      public:
        /**
         * @brief Creates service descriptor
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
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeSingleton<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeSingleton<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeSingleton()
        {
            return describe<TService, TImplementation>(ServiceLifeTimes::Singleton);
        }

        /**
         * @brief Creates service descriptor
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
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeScoped<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeScoped<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeScoped()
        {
            return describe<TService, TImplementation>(ServiceLifeTimes::Scoped);
        }
        /**
         * @brief Creates service descriptor
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
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeTransient<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeTransient<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeTransient()
        {
            return describe<TService, TImplementation>(ServiceLifeTimes::Transient);
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describe<TestClass>(ServiceLifeTime::scoped());
         * ServiceDescriptor descriptor2 =
         *          ServiceDescriber::describe<BaseClass, ImplementationClass>(ServiceLifeTime::transient());
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(const ServiceLifeTime lifetime)
        {
            details::utils::Assert::inheritance<TService, TImplementation>();
            auto factory = std::make_unique<details::factories::ServiceFactory<TImplementation>>();
            return {typeid(TService), lifetime, std::move(factory)};
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external service pointer
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
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
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external service pointer
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
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
            details::utils::Assert::inheritance<TService, TImplementation>();
            auto factory = std::make_unique<details::factories::ExternalServiceFactory<TImplementation>>(service);
            return {typeid(TService), ServiceLifeTimes::Singleton, std::move(factory)};
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTimes::Singleton, std::forward<FactoryFcn>(factory));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */

        template <class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTimes::Scoped, std::forward<FactoryFcn>(factory));
        }
        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */

        template <class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTimes::Transient, std::forward<FactoryFcn>(factory));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom(
         *       []() { return std::make_unique<TestClass>(); }, ServiceLifeTime::scoped());
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeFrom(const ServiceLifeTime lifetime, FactoryFcn &&factoryFcn)
        {
            return describeFrom<void, FactoryFcn>(lifetime, std::forward<FactoryFcn>(factoryFcn));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers, implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTimes::Singleton, std::forward<FactoryFcn>(factory));
        }
        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers, implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTimes::Scoped, std::forward<FactoryFcn>(factory));
        }
        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers, implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTimes::Transient, std::forward<FactoryFcn>(factory));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers, implementation type must inherit from TService
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); }, ServiceLifeTime::scoped());
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFrom(const ServiceLifeTime lifetime, FactoryFcn &&factoryFcn)
        {
            auto factory = makeFactoryFrom<TService, FactoryFcn>(std::forward<FactoryFcn>(factoryFcn));
            auto serviceTypeId = std::is_void_v<TService> ? factory->getServiceTypeId() : TypeId(typeid(TService));
            return {serviceTypeId, lifetime, std::move(factory)};
        }

        template <class TAlias, class TService> static ServiceDescriptor describeAlias()
        {
            details::utils::Assert::isNotSame<TAlias, TService>();
            details::utils::Assert::inheritance<TAlias, TService>();
            auto factory = std::make_unique<details::factories::VoidServiceFactory<TService>>();
            return {typeid(TAlias), ServiceLifeTimes::Scoped, std::move(factory), true};
        }

      private:
        template <class TService, class FactoryFcn> static IServiceFactory::Ptr makeFactoryFrom(FactoryFcn &&factoryFcn)
        {
            using ReturnType = typename details::helpers::ServiceFactoryInvoker<FactoryFcn>::ReturnType;
            if constexpr (details::utils::IsUniquePtrV<ReturnType>)
            {
                using Factory = details::factories::UniquePtrServiceFcnFactory<FactoryFcn>;
                tryCheckFactoryInheritance<TService, Factory>();
                return std::make_unique<Factory>(std::forward<FactoryFcn>(factoryFcn));
            }
            else if constexpr (details::utils::IsInPlaceObjectConstructableV<ReturnType>)
            {
                using Factory = details::factories::ServiceFcnFactory<FactoryFcn>;
                tryCheckFactoryInheritance<TService, Factory>();
                return std::make_unique<Factory>(std::forward<FactoryFcn>(factoryFcn));
            }
            else
            {
                notSupportedReturnType<FactoryFcn>();
                return nullptr;
            }
        }

        template <class TService, class Factory> static void tryCheckFactoryInheritance()
        {
            if constexpr (!std::is_void_v<TService>)
            {
                details::utils::Assert::inheritance<TService, typename Factory::ServiceType>();
            }
        }

        template <class FactoryFcn> static void notSupportedReturnType()
        {
            static_assert(details::utils::notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or movable/copyable object");
        }
    };
} // namespace sb::di
