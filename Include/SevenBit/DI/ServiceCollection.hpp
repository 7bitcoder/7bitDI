#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Container.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    class EXPORT ServiceCollection
    {
      private:
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        using Iterator = std::vector<ServiceDescriptor>::iterator;
        using ConstIterator = std::vector<ServiceDescriptor>::const_iterator;
        using ReverseIterator = std::vector<ServiceDescriptor>::reverse_iterator;
        using ConstReverseIterator = std::vector<ServiceDescriptor>::const_reverse_iterator;

        using Ptr = std::unique_ptr<ServiceCollection>;

        ServiceCollection() = default;
        ServiceCollection(const ServiceCollection &) = default;
        ServiceCollection(ServiceCollection &&) = default;

        ServiceCollection &operator=(const ServiceCollection &) = default;
        ServiceCollection &operator=(ServiceCollection &&) = default;

        /**
         * @brief Builds instanceValidity provider with specified options
         * @details might throw exceptions
         * @throws ServiceAlreadyRegisteredException if instanceValidity was already registered
         * @throws ServiceLifeTimeMissmatchException if instanceValidity has different lifetime than other already
         * registered with same base type
         */
        ServiceProvider::Ptr buildServiceProvider(ServiceProviderOptions options = {});

        Iterator begin() { return _serviceDescriptors.begin(); }
        Iterator end() { return _serviceDescriptors.end(); }

        ConstIterator cBegin() const { return _serviceDescriptors.cbegin(); }
        ConstIterator cEnd() const { return _serviceDescriptors.cend(); }

        ConstIterator begin() const { return cBegin(); }
        ConstIterator end() const { return cEnd(); }

        ReverseIterator rBegin() { return _serviceDescriptors.rbegin(); }
        ReverseIterator rEnd() { return _serviceDescriptors.rend(); }

        ConstReverseIterator crBegin() const { return _serviceDescriptors.crbegin(); }
        ConstReverseIterator crEnd() const { return _serviceDescriptors.crend(); }

        ConstReverseIterator rbegin() const { return crBegin(); }
        ConstReverseIterator rend() const { return crEnd(); }
        /**
         * @brief Returns instanceValidity descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &at(size_t index);
        /**
         * @brief Returns instanceValidity descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        const ServiceDescriptor &at(size_t index) const;

        /**
         * @brief Returns first descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        ServiceDescriptor &first();
        /**
         * @brief Returns first descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        const ServiceDescriptor &first() const;

        /**
         * @brief Returns last descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        ServiceDescriptor &last();
        /**
         * @brief Returns last descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        const ServiceDescriptor &last() const;

        /**
         * @brief Returns instanceValidity descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &operator[](size_t index);
        /**
         * @brief Returns instanceValidity descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        const ServiceDescriptor &operator[](size_t index) const;

        /**
         * @brief Returns number of stored descriptors
         */
        size_t size() const;
        /**
         * @brief Returns number of stored descriptors
         */
        size_t count() const;

        /**
         * @brief Returns true if there are no descriptors
         */
        bool empty() const;

        /**
         * @brief Returns capacity
         */
        size_t capacity() const;

        /**
         * @brief Reserves space for descriptors
         */
        void reserve(size_t space);

        /**
         * @brief Shrinks to fit current size of descriptors
         */
        void shrinkToFit();

        /**
         * @brief Removes all descriptors
         */
        void clear();

        /**
         * @brief Find first descriptor meeting TPred requirement
         * @tparam TPred is functor with this sheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> Iterator findIf(const TPred &pred) { return std::find_if(begin(), end(), pred); }

        /**
         * @brief Find first descriptor meeting TPred requirement
         * @tparam TPred is functor with this sheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> ConstIterator findIf(const TPred &pred) const
        {
            return std::find_if(begin(), end(), pred);
        }

        /**
         * @brief Cheks if contains descriptor meeting TPred requirement
         * @tparam TPred is functor with this sheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> bool containsIf(const TPred &pred) const { return findIf(pred) != end(); }

        /**
         * @brief Cheks if contains descriptor matching requirement
         * @code {.cpp}
         * descriptor.getServiceTypeId() == typeid(TService)
         * @endcode
         */
        template <class TService> bool contains() const { return contains(typeid(TService)); }

        /**
         * @brief Cheks if contains descriptor matching requirement
         * @code {.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId
         * @endcode
         */
        bool contains(TypeId serviceTypeId) const;

        /**
         * @brief Cheks if contains descriptor matching requirement
         * @code {.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService)
         * @endcode
         */
        template <class TService, class TImplementation = TService> bool containsExact() const
        {
            return containsExact(typeid(TService), typeid(TImplementation));
        }

        /**
         * @brief Cheks if contains descriptor matching requirement
         * @code {.cpp}
         * descriptor.getImplementationTypeId() == implementationTypeId && descriptor.getServiceTypeId() ==
         * @endcode
         * serviceTypeId
         */
        bool containsExact(TypeId serviceTypeId, TypeId implementationTypeId) const;

        /**
         * @brief Inserts descriptor before giver iterator
         * @details Returns iterator pointing to the inserted
         */
        Iterator insert(ConstIterator pos, ServiceDescriptor descriptor);

        /**
         * @brief Adds descriptor to the end of list
         */
        ServiceCollection &add(ServiceDescriptor descriptor);

        /**
         * @brief Adds descriptors from collection to the end of list
         */
        ServiceCollection &add(const ServiceCollection &collection);

        /**
         * @brief Removes descriptor with given iterator
         * @details Returns iterator following the last removed element
         */
        Iterator remove(Iterator pos);
        /**
         * @brief Removes descriptor with given iterator
         * @details Returns iterator following the last removed element
         */
        Iterator remove(ConstIterator pos);

        /**
         * @brief Removes descriptors between given iterators
         * @details Returns iterator following the last removed element
         */
        Iterator removeRange(Iterator begin, Iterator end);
        /**
         * @brief Removes descriptors between given iterators
         * @details Returns iterator following the last removed element
         */
        Iterator removeRange(ConstIterator begin, ConstIterator end);

        /**
         * @brief Removes all descriptors meeting TPred requirement
         * @tparam TPred is functor with this sheme: (const ServiceDescriptor&) -> bool
         * @details Returns number of removed elements
         */
        template <class TPred> size_t removeIf(const TPred &pred)
        {
            auto it = details::utils::Container::removeIf(begin(), end(), pred);
            auto r = std::distance(it, end());
            removeRange(it, end());
            return r;
        }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code {.cpp}
         * descriptor.getServiceTypeId() == typeid(TService)
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService> size_t removeAll() { return removeAll(typeid(TService)); }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code {.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId
         * @endcode
         * @details Returns number of removed elements
         */
        size_t removeAll(TypeId serviceTypeId);

        /**
         * @brief Removes all descriptors meeting requirement
         * @code {.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService)
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService, class TImplementation = TService> size_t remove()
        {
            return remove(typeid(TService), typeid(TImplementation));
        }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code {.cpp}
         * descriptor.getImplementationTypeId() == implementationTypeId && descriptor.getServiceTypeId() ==
         * serviceTypeId
         * @endcode
         * @details Returns number of removed elements
         */
        size_t remove(TypeId serviceTypeId, TypeId implementationTypeId);

        /**
         * @brief Removes last descriptor
         */
        void pop();

        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - given lifetime,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base instanceValidity type
         * @tparam TImplementation instanceValidity implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.add<TestClass>(ServiceLifeTime::scoped());
         * ServiceCollection{}.add<BaseClass, ImplementationClass>(ServiceLifeTime::transient());
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &add(ServiceLifeTime lifeTime)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(lifeTime));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base instanceValidity type
         * @tparam TImplementation instanceValidity implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addSingleton<TestClass>();
         * ServiceCollection{}.addSingleton<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addSingleton()
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>());
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base instanceValidity type
         * @tparam TImplementation instanceValidity implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addScoped<TestClass>();
         * ServiceCollection{}.addScoped<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addScoped()
        {
            return add(ServiceDescriber::describeScoped<TService, TImplementation>());
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - default factory using TImplementation constructor
         * @tparam TService base instanceValidity type
         * @tparam TImplementation instanceValidity implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addTransient<TestClass>();
         * ServiceCollection{}.addTransient<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addTransient()
        {
            return add(ServiceDescriber::describeTransient<TService, TImplementation>());
        }

        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TService),
         * factory - factory with external instanceValidity pointner
         * @tparam TService instanceValidity type must have one constructor
         *
         * Example:
         * @code {.cpp}
         * TestClass test;
         * ServiceCollection{}.addSingleton(&test);
         * @endcode
         */
        template <class TService> ServiceCollection &addSingleton(TService *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TService>(service));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external instanceValidity pointner
         * @tparam TService base instanceValidity type
         * @tparam TImplementation instanceValidity implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         *
         * Example:
         * @code {.cpp}
         * ImplementationClass implementation;
         * ServiceCollection{}.addSingleton<BaseClass>(&implementation);
         * @endcode
         */
        template <class TService, class TImplementation> ServiceCollection &addSingleton(TImplementation *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>(service));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - given instanceValidity lifetime,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.add<BaseClass>(ServiceLifeTime::scoped(),
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &add(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom<TService, FactoryFcn>(lifeTime, std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addSingleton<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom<TService, FactoryFcn>(std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addScoped<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom<TService, FactoryFcn>(std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addTransient<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom<TService, FactoryFcn>(std::move(factory)));
        }

        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - given instanceValidity lifetime,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.add(ServiceLifeTime::transient(), []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &add(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom(lifeTime, std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - singleton,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addSingleton([]() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom(std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - scoped,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addScoped([]() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom(std::move(factory)));
        }
        /**
         * @brief Adds ServiceDescriptor
         * @details Adds instanceValidity descriptor with:
         * lifetime - transient,
         * serviceTypeId - extracted from factory return type,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base instanceValidity type
         * @tparam FactoryFcn is factory functor with this sheme: (Services...) ->
         * std::unique_ptr<TImplementation>, where services are pointners, unique pointners, references, vectors with
         * pointners or unique pointners
         *
         * Example:
         * @code {.cpp}
         * ServiceCollection{}.addTransient([]() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom(std::move(factory)));
        }
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceCollection.hpp"
#endif
