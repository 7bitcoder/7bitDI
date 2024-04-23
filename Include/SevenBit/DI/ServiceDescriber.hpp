#pragma once

#include <type_traits>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Factories/ExternalServiceFactory.hpp>
#include <SevenBit/DI/Details/Factories/ServiceFactory.hpp>
#include <SevenBit/DI/Details/Factories/ServiceFcnFactory.hpp>
#include <SevenBit/DI/Details/Utils/Assert.hpp>
#include <SevenBit/DI/Details/Utils/Cast.hpp>
#include <SevenBit/DI/ServiceDescriptor.hpp>
#include <SevenBit/DI/ServiceLifeTimes.hpp>

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
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using TImplementation constructor,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeSingleton<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeSingleton<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeSingleton(std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::singleton(), std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using TImplementation constructor,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeScoped<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeScoped<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeScoped(std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::scoped(), std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using TImplementation constructor,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describeTransient<TestClass>();
         * ServiceDescriptor descriptor2 = ServiceDescriber::describeTransient<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeTransient(std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::transient(), std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using TImplementation constructor,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor1 = ServiceDescriber::describe<TestClass>(ServiceLifeTime::scoped());
         * ServiceDescriptor descriptor2 =
         *          ServiceDescriber::describe<BaseClass, ImplementationClass>(ServiceLifeTime::transient());
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(const ServiceLifeTime lifeTime,
                                          std::unique_ptr<std::string> serviceKey = nullptr)
        {
            details::Assert::serviceTypes<TService, TImplementation>();

            return {typeid(TService),
                    typeid(TImplementation),
                    std::move(serviceKey),
                    nullptr,
                    lifeTime,
                    std::make_unique<details::ServiceFactory<TImplementation>>(),
                    details::Cast::getCastOffset<TService, TImplementation>()};
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - factory with external service pointer,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         * @param serviceKey service key can be empty or nullptr to describe default service
         * @param service external service pointer
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
        static ServiceDescriptor describeSingleton(TImplementation &service,
                                                   std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeSingleton<TService, TImplementation>(&service, std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - factory with external service pointer,
         * castOffset - cast offset between TImplementation and TService in bytes,
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         * @param serviceKey service key can be empty or nullptr to describe default service
         * @param service external service pointer
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
        static ServiceDescriptor describeSingleton(TImplementation *service,
                                                   std::unique_ptr<std::string> serviceKey = nullptr)
        {
            details::Assert::serviceTypes<TService, TImplementation>();

            return {typeid(TService),
                    typeid(TImplementation),
                    std::move(serviceKey),
                    nullptr,
                    ServiceLifeTime::singleton(),
                    std::make_unique<details::ExternalServiceFactory<TImplementation>>(service),
                    details::Cast::getCastOffset<TService, TImplementation>()};
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory,
                                                       std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::singleton(), std::forward<FactoryFcn>(factory),
                                                  std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory,
                                                    std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::scoped(), std::forward<FactoryFcn>(factory),
                                                  std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom(
         *       [](const ServiceDescriptor &) { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory,
                                                       std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::transient(), std::forward<FactoryFcn>(factory),
                                                  std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom(ServiceLifeTime::scoped(),
         *       []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn>
        static ServiceDescriptor describeFrom(const ServiceLifeTime lifeTime, FactoryFcn &&factory,
                                              std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<void, FactoryFcn>(lifeTime, std::forward<FactoryFcn>(factory), std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeSingletonFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeSingletonFrom(FactoryFcn &&factory,
                                                       std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::singleton(), std::forward<FactoryFcn>(factory),
                                                      std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeScopedFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeScopedFrom(FactoryFcn &&factory,
                                                    std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::scoped(), std::forward<FactoryFcn>(factory),
                                                      std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeTransientFrom<BaseClass>(
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeTransientFrom(FactoryFcn &&factory,
                                                       std::unique_ptr<std::string> serviceKey = nullptr)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::transient(), std::forward<FactoryFcn>(factory),
                                                      std::move(serviceKey));
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - given service lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * serviceKey - service key passed,
         * implementationKey - nullptr,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeFrom<BaseClass>(ServiceLifeTime::scoped(),
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFrom(const ServiceLifeTime lifeTime, FactoryFcn &&factory,
                                              std::unique_ptr<std::string> serviceKey = nullptr)
        {
            using Factory = details::ServiceFcnFactory<FactoryFcn>;
            using TImplementation = typename Factory::ServiceType;
            using TFinalService = std::conditional_t<std::is_void_v<TService>, TImplementation, TService>;
            details::Assert::factoryTypes<TFinalService, TImplementation>();

            return {typeid(TFinalService),
                    typeid(TImplementation),
                    std::move(serviceKey),
                    nullptr,
                    lifeTime,
                    std::make_unique<Factory>(std::forward<FactoryFcn>(factory)),
                    details::Cast::getCastOffset<TFinalService, TImplementation>()};
        }

        /**
         * @brief Creates service descriptor
         * @details Creates service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TAlias),
         * implementationTypeId - typeid(TService),
         * serviceKey - service alias key passed,
         * implementationKey - service key passed,
         * factory - default factory using FactoryFcn factory functor,
         * castOffset - cast offset between TImplementation and TService in bytes
         * @tparam TAlias base service type - alias type
         * @tparam TService service type must inherit from TService
         * @param serviceAliasKey alias service key can be empty to describe default alias service
         * @param serviceKey service key can be empty or nullptr to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceDescriptor descriptor = ServiceDescriber::describeAlias<AliasClass, ServiceClass>();
         * @endcode
         */
        template <class TAlias, class TService>
        static ServiceDescriptor describeAlias(std::unique_ptr<std::string> serviceAliasKey = nullptr,
                                               std::unique_ptr<std::string> serviceKey = nullptr)
        {
            details::Assert::aliasTypes<TAlias, TService>();

            return {typeid(TAlias),
                    typeid(TService),
                    std::move(serviceAliasKey),
                    std::move(serviceKey),
                    ServiceLifeTime::scoped(),
                    nullptr,
                    details::Cast::getCastOffset<TAlias, TService>()};
        }
    };
} // namespace sb::di
