#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Container.hpp"
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
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        using Iterator = std::vector<ServiceDescriptor>::iterator;
        using ConstIterator = std::vector<ServiceDescriptor>::const_iterator;
        using ReverseIterator = std::vector<ServiceDescriptor>::reverse_iterator;
        using ConstReverseIterator = std::vector<ServiceDescriptor>::const_reverse_iterator;

        using Ptr = std::unique_ptr<ServiceCollection>;

        ServiceCollection() = default;
        ServiceCollection(std::initializer_list<ServiceDescriptor> list);
        template <class InputIt> ServiceCollection(InputIt first, InputIt last) : _serviceDescriptors(first, last) {}

        ServiceCollection(const ServiceCollection &) = default;
        ServiceCollection(ServiceCollection &&) = default;

        ServiceCollection &operator=(const ServiceCollection &) = default;
        ServiceCollection &operator=(ServiceCollection &&) = default;

        /**
         * @brief Builds service provider with specified options
         * @details might throw exceptions
         * @throws sb::di::ServiceAlreadyRegisteredException if service was already registered
         * @throws sb::di::ServiceLifeTimeMismatchException if service has different lifetime than other already
         * registered with same base type
         */
        ServiceProvider buildServiceProvider(ServiceProviderOptions options = {});

        /**
         * @brief Builds service provider as unique_ptr with specified options
         * @details might throw exceptions
         * @throws sb::di::ServiceAlreadyRegisteredException if service was already registered
         * @throws sb::di::ServiceLifeTimeMismatchException if service has different lifetime than other already
         * registered with same base type
         */
        ServiceProvider::Ptr buildServiceProviderAsPtr(ServiceProviderOptions options = {});

        Iterator begin() { return _serviceDescriptors.begin(); }
        Iterator end() { return _serviceDescriptors.end(); }

        [[nodiscard]] ConstIterator cBegin() const { return _serviceDescriptors.cbegin(); }
        [[nodiscard]] ConstIterator cEnd() const { return _serviceDescriptors.cend(); }

        [[nodiscard]] ConstIterator begin() const { return cBegin(); }
        [[nodiscard]] ConstIterator end() const { return cEnd(); }

        ReverseIterator rBegin() { return _serviceDescriptors.rbegin(); }
        ReverseIterator rEnd() { return _serviceDescriptors.rend(); }

        [[nodiscard]] ConstReverseIterator crBegin() const { return _serviceDescriptors.crbegin(); }
        [[nodiscard]] ConstReverseIterator crEnd() const { return _serviceDescriptors.crend(); }

        [[nodiscard]] ConstReverseIterator rbegin() const { return crBegin(); }
        [[nodiscard]] ConstReverseIterator rend() const { return crEnd(); }

        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &at(size_t index);
        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        [[nodiscard]] const ServiceDescriptor &at(size_t index) const;

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
        [[nodiscard]] const ServiceDescriptor &first() const;

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
        [[nodiscard]] const ServiceDescriptor &last() const;

        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &operator[](size_t index);
        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        const ServiceDescriptor &operator[](size_t index) const;

        /**
         * @brief Returns number of stored descriptors
         */
        [[nodiscard]] size_t size() const;
        /**
         * @brief Returns number of stored descriptors
         */
        [[nodiscard]] size_t count() const;

        /**
         * @brief Returns true if there are no descriptors
         */
        [[nodiscard]] bool empty() const;

        /**
         * @brief Returns capacity
         */
        [[nodiscard]] size_t capacity() const;

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
         * @tparam TPred is functor with this scheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> Iterator findIf(TPred pred) { return std::find_if(begin(), end(), std::move(pred)); }

        /**
         * @brief Find first descriptor meeting TPred requirement
         * @tparam TPred is functor with this scheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> [[nodiscard]] ConstIterator findIf(TPred pred) const
        {
            return std::find_if(begin(), end(), std::move(pred));
        }

        /**
         * @brief Checks if contains descriptor meeting TPred requirement
         * @tparam TPred is functor with this scheme: (const ServiceDescriptor&) -> bool
         */
        template <class TPred> [[nodiscard]] bool containsIf(TPred pred) const
        {
            return findIf(std::move(pred)) != end();
        }

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == typeid(TService)
         * @endcode
         */
        template <class TService> [[nodiscard]] bool contains() const { return contains(typeid(TService)); }

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId
         * @endcode
         */
        [[nodiscard]] bool contains(TypeId serviceTypeId) const;

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService)
         * @endcode
         */
        template <class TService, class TImplementation = TService> [[nodiscard]] bool containsExact() const
        {
            return containsExact(typeid(TService), typeid(TImplementation));
        }

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == implementationTypeId && descriptor.getServiceTypeId() ==
         * serviceTypeId
         * @endcode
         */
        [[nodiscard]] bool containsExact(TypeId serviceTypeId, TypeId implementationTypeId) const;

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
         * @tparam TPred is functor with this scheme: (const ServiceDescriptor&) -> bool
         * @details Returns number of removed elements
         */
        template <class TPred> size_t removeIf(TPred pred)
        {
            auto it = details::utils::Container::removeIf(begin(), end(), std::move(pred));
            auto r = std::distance(it, end());
            removeRange(it, end());
            return r;
        }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == typeid(TService)
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService> size_t removeAll() { return removeAll(typeid(TService)); }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId
         * @endcode
         * @details Returns number of removed elements
         */
        size_t removeAll(TypeId serviceTypeId);

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
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
         * @code{.cpp}
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
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
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
         * ServiceCollection{}.add<TestClass>(ServiceLifeTime::scoped());
         * ServiceCollection{}.add<BaseClass, ImplementationClass>(ServiceLifeTime::transient());
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &add(const ServiceLifeTime lifeTime)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(lifeTime));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - singleton,
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
         * ServiceCollection{}.addSingleton<TestClass>();
         * ServiceCollection{}.addSingleton<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addSingleton()
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>());
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - scoped,
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
         * ServiceCollection{}.addScoped<TestClass>();
         * ServiceCollection{}.addScoped<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addScoped()
        {
            return add(ServiceDescriber::describeScoped<TService, TImplementation>());
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - transient,
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
         * ServiceCollection{}.addTransient<TestClass>();
         * ServiceCollection{}.addTransient<BaseClass, ImplementationClass>();
         * @endcode
         */
        template <class TService, class TImplementation = TService> ServiceCollection &addTransient()
        {
            return add(ServiceDescriber::describeTransient<TService, TImplementation>());
        }

        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TService),
         * factory - factory with external service pointer
         * @tparam TService service type
         *
         * Example:
         * @code{.cpp}
         * TestClass test;
         * ServiceCollection{}.addSingleton(&test);
         * @endcode
         */
        template <class TService> ServiceCollection &addSingleton(TService *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TService>(service));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - typeid(TImplementation),
         * factory - factory with external service pointer
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         * @see Constructor requirements
         *
         * Example:
         * @code{.cpp}
         * ImplementationClass implementation;
         * ServiceCollection{}.addSingleton<BaseClass>(&implementation);
         * @endcode
         */
        template <class TService, class TImplementation> ServiceCollection &addSingleton(TImplementation *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>(service));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - given service lifetime,
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add<BaseClass>(ServiceLifeTime::scoped(),
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &add(const ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom<TService, FactoryFcn>(lifeTime, std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - singleton,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addSingleton<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom<TService, FactoryFcn>(std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - scoped,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addScoped<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom<TService, FactoryFcn>(std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
         * lifetime - transient,
         * serviceTypeId - typeid(TService),
         * implementationTypeId - extracted from factory return type,
         * factory - default factory using FactoryFcn factory functor
         * @tparam TService base service type
         * @tparam FactoryFcn is factory functor with this scheme: (Services...) ->
         * std::unique_ptr<TImplementation> | TImplementation, where services are pointers, unique pointers, references,
         * vectors with pointers or unique pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addTransient<BaseClass>([]() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom<TService, FactoryFcn>(std::move(factory)));
        }

        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
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
         * ServiceCollection{}.add(ServiceLifeTime::transient(), []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &add(const ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom(lifeTime, std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
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
         * ServiceCollection{}.addSingleton([]() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom(std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
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
         * ServiceCollection{}.addScoped([]() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom(std::move(factory)));
        }
        /**
         * @brief Adds service descriptor
         * @details Adds service descriptor with:
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
