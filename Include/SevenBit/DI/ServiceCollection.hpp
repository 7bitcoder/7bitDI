#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Utils/Container.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>
#include <SevenBit/DI/ServiceLifeTimes.hpp>
#include <SevenBit/DI/ServiceProvider.hpp>
#include <SevenBit/DI/ServiceProviderOptions.hpp>

namespace sb::di
{
    struct ServiceProviderOptions;
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

        ServiceCollection(const std::initializer_list<ServiceDescriptor> list) : _serviceDescriptors(list) {}

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

        [[nodiscard]] ConstReverseIterator rBegin() const { return crBegin(); }
        [[nodiscard]] ConstReverseIterator rEnd() const { return crEnd(); }

        /**
         * @brief Returns inner vector container
         */
        std::vector<ServiceDescriptor> &getInnerVector() { return _serviceDescriptors; }
        /**
         * @brief Returns inner vector container
         */
        [[nodiscard]] const std::vector<ServiceDescriptor> &getInnerVector() const { return _serviceDescriptors; }

        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &at(const std::size_t index) { return _serviceDescriptors.at(index); }
        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        [[nodiscard]] const ServiceDescriptor &at(const std::size_t index) const
        {
            return _serviceDescriptors.at(index);
        }

        /**
         * @brief Returns first descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        ServiceDescriptor &first() { return at(0); }
        /**
         * @brief Returns first descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        [[nodiscard]] const ServiceDescriptor &first() const { return at(0); }

        /**
         * @brief Returns last descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        ServiceDescriptor &last() { return at(size() - 1); }
        /**
         * @brief Returns last descriptor
         * @details might throw exception
         * @throws std::out_of_range if empty()
         */
        [[nodiscard]] const ServiceDescriptor &last() const { return at(size() - 1); }

        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        ServiceDescriptor &operator[](const std::size_t index) { return _serviceDescriptors[index]; }
        /**
         * @brief Returns service descriptor at giver position
         * @details might throw exception
         * @throws std::out_of_range if index >= size()
         */
        const ServiceDescriptor &operator[](const std::size_t index) const { return _serviceDescriptors[index]; }

        /**
         * @brief Returns the maximum possible number of stored descriptors
         */
        [[nodiscard]] std::size_t maxSize() const { return _serviceDescriptors.max_size(); }

        /**
         * @brief Returns number of stored descriptors
         */
        [[nodiscard]] std::size_t size() const { return _serviceDescriptors.size(); }
        /**
         * @brief Returns number of stored descriptors
         */
        [[nodiscard]] std::size_t count() const { return size(); }

        /**
         * @brief Returns true if there are no descriptors
         */
        [[nodiscard]] bool empty() const { return _serviceDescriptors.empty(); }

        /**
         * @brief Returns capacity
         */
        [[nodiscard]] std::size_t capacity() const { return _serviceDescriptors.capacity(); }

        /**
         * @brief Reserves space for descriptors
         */
        void reserve(const std::size_t space) { _serviceDescriptors.reserve(space); }

        /**
         * @brief Shrinks to fit current size of descriptors
         */
        void shrinkToFit() { _serviceDescriptors.shrink_to_fit(); }

        /**
         * @brief Removes all descriptors
         */
        void clear() { _serviceDescriptors.clear(); }

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
         * descriptor.getServiceTypeId() == typeid(TService) && *descriptor.getServiceKey() == serviceKey
         * @endcode
         */
        template <class TService> [[nodiscard]] bool containsKeyed(const std::string_view serviceKey) const
        {
            return containsKeyed(typeid(TService), serviceKey);
        }

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId && *descriptor.getServiceKey() == serviceKey
         * @endcode
         */
        [[nodiscard]] bool containsKeyed(TypeId serviceTypeId, std::string_view serviceKey) const;

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
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService) && *descriptor.getServiceKey() == serviceKey
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        [[nodiscard]] bool containsKeyedExact(const std::string_view serviceKey) const
        {
            return containsKeyedExact(typeid(TService), typeid(TImplementation), serviceKey);
        }

        /**
         * @brief Checks if contains descriptor matching requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == implementationTypeId && descriptor.getServiceTypeId() ==
         * serviceTypeId && *descriptor.getServiceKey() == serviceKey
         * @endcode
         */
        [[nodiscard]] bool containsKeyedExact(TypeId serviceTypeId, TypeId implementationTypeId,
                                              std::string_view serviceKey) const;

        /**
         * @brief Inserts descriptor before giver iterator
         * @details Returns iterator pointing to the inserted
         */
        Iterator insert(ConstIterator pos, ServiceDescriptor descriptor)
        {
            return _serviceDescriptors.insert(pos, std::move(descriptor));
        }

        /**
         * @brief Adds descriptor to the end of list
         */
        ServiceCollection &add(ServiceDescriptor descriptor);

        /**
         * @brief Adds descriptors from collection to the end of list
         */
        ServiceCollection &add(const ServiceCollection &collection);

        /**
         * @brief Adds descriptors from collection to the end of list, that matches filter functor
         */
        template <class TFilter> ServiceCollection &addWithFilter(const ServiceCollection &collection, TFilter filter)
        {
            reserve(size() + collection.size());
            for (auto &descriptor : collection)
            {
                if (filter(descriptor))
                {
                    add(descriptor);
                }
            }
            return *this;
        }

        /**
         * @brief Removes descriptor with given iterator
         * @details Returns iterator following the last removed element
         */
        Iterator remove(Iterator pos) { return _serviceDescriptors.erase(pos); }

        /**
         * @brief Removes descriptor with given iterator
         * @details Returns iterator following the last removed element
         */
        Iterator remove(ConstIterator pos) { return _serviceDescriptors.erase(pos); }

        /**
         * @brief Removes descriptors between given iterators
         * @details Returns iterator following the last removed element
         */
        Iterator removeRange(Iterator begin, Iterator end) { return _serviceDescriptors.erase(begin, end); }

        /**
         * @brief Removes descriptors between given iterators
         * @details Returns iterator following the last removed element
         */
        Iterator removeRange(ConstIterator begin, ConstIterator end) { return _serviceDescriptors.erase(begin, end); }

        /**
         * @brief Removes all descriptors meeting TPred requirement
         * @tparam TPred is functor with this scheme: (const ServiceDescriptor&) -> bool
         * @details Returns number of removed elements
         */
        template <class TPred> std::size_t removeIf(TPred pred)
        {
            auto it = details::Container::removeIf(begin(), end(), std::move(pred));
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
        template <class TService> std::size_t removeAll() { return removeAll(typeid(TService)); }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId
         * @endcode
         * @details Returns number of removed elements
         */
        std::size_t removeAll(TypeId serviceTypeId);

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == typeid(TService) && *descriptor.getServiceKey() == serviceKey
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService> std::size_t removeAllKeyed(const std::string_view serviceKey)
        {
            return removeAllKeyed(typeid(TService), serviceKey);
        }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getServiceTypeId() == serviceTypeId && *descriptor.getServiceKey() == serviceKey
         * @endcode
         * @details Returns number of removed elements
         */
        std::size_t removeAllKeyed(TypeId serviceTypeId, std::string_view serviceKey);

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService)
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService, class TImplementation = TService> std::size_t remove()
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
        std::size_t remove(TypeId serviceTypeId, TypeId implementationTypeId);

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == typeid(TImplementation) && descriptor.getServiceTypeId() ==
         * typeid(TService) && *descriptor.getServiceKey() == serviceKey
         * @endcode
         * @details Returns number of removed elements
         */
        template <class TService, class TImplementation = TService>
        std::size_t removeKeyed(const std::string_view serviceKey)
        {
            return removeKeyed(typeid(TService), typeid(TImplementation), serviceKey);
        }

        /**
         * @brief Removes all descriptors meeting requirement
         * @code{.cpp}
         * descriptor.getImplementationTypeId() == implementationTypeId && descriptor.getServiceTypeId() ==
         * serviceTypeId && *descriptor.getServiceKey() == serviceKey
         * @endcode
         * @details Returns number of removed elements
         */
        std::size_t removeKeyed(TypeId serviceTypeId, TypeId implementationTypeId, std::string_view serviceKey);

        /**
         * @brief Removes last descriptor
         */
        void pop() { _serviceDescriptors.pop_back(); }

        /**
         * @brief Adds service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param lifeTime service life time Singleton Scoped or Transient
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add<TestClass>(ServiceLifeTimes::Scoped);
         * ServiceCollection{}.add<BaseClass, ImplementationClass>(ServiceLifeTimes::Transient);
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &add(const ServiceLifeTime lifeTime)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(lifeTime));
        }

        /**
         * @brief Adds keyed service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param serviceKey service key can be empty to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add<TestClass>(ServiceLifeTimes::Scoped, "key");
         * ServiceCollection{}.add<BaseClass, ImplementationClass>(ServiceLifeTimes::Transient, "key");
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &addKeyed(const ServiceLifeTime lifeTime, std::string serviceKey)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(
                lifeTime, std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds singleton service descriptor
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
         * @brief Adds keyed singleton service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addSingleton<TestClass>("key");
         * ServiceCollection{}.addSingleton<BaseClass, ImplementationClass>("key");
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &addKeyedSingleton(std::string serviceKey)
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>(
                std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds scoped service descriptor
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
         * @brief Adds keyed scoped service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addScoped<TestClass>("key");
         * ServiceCollection{}.addScoped<BaseClass, ImplementationClass>("key");
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &addKeyedScoped(std::string serviceKey)
        {
            return add(ServiceDescriber::describeScoped<TService, TImplementation>(
                std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds transient service descriptor
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
         * @brief Adds keyed transient service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService and must have one
         * constructor
         * @see Constructor requirements
         * @param serviceKey service key can be empty to describe default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addTransient<TestClass>("key");
         * ServiceCollection{}.addTransient<BaseClass, ImplementationClass>("key");
         * @endcode
         */
        template <class TService, class TImplementation = TService>
        ServiceCollection &addKeyedTransient(std::string serviceKey)
        {
            return add(ServiceDescriber::describeTransient<TService, TImplementation>(
                std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds singleton service descriptor
         * @tparam TService service type
         * @param service external service pointer
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
         * @brief Adds keyed singleton service descriptor
         * @tparam TService service type
         * @param serviceKey service key can be empty to describe default service
         * @param service external service pointer
         *
         * Example:
         * @code{.cpp}
         * TestClass test;
         * ServiceCollection{}.addSingleton("key", &test);
         * @endcode
         */
        template <class TService> ServiceCollection &addKeyedSingleton(std::string serviceKey, TService *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TService>(
                service, std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds singleton service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         * @see Constructor requirements
         * @param service external service pointer
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
         * @brief Adds keyed singleton service descriptor
         * @tparam TService base service type
         * @tparam TImplementation service implementation type must inherit from TService
         * @see Constructor requirements
         * @param serviceKey service key can be empty to describe default service
         * @param service external service pointer
         *
         * Example:
         * @code{.cpp}
         * ImplementationClass implementation;
         * ServiceCollection{}.addSingleton<BaseClass>("key", &implementation);
         * @endcode
         */
        template <class TService, class TImplementation>
        ServiceCollection &addKeyedSingleton(std::string serviceKey, TImplementation *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>(
                service, std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add<BaseClass>(ServiceLifeTimes::Scoped,
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &add(const ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom<TService, FactoryFcn>(lifeTime, std::move(factory)));
        }

        /**
         * @brief Adds keyed service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add<BaseClass>(ServiceLifeTimes::Scoped, "key",
         *       []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &addKeyed(const ServiceLifeTime lifeTime, std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom<TService, FactoryFcn>(
                lifeTime, std::move(factory), std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds singleton service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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
         * @brief Adds keyed singleton service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addSingleton<BaseClass>("key", []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &addKeyedSingleton(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom<TService, FactoryFcn>(
                std::move(factory), std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds scoped service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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
         * @brief Adds keyed scoped service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addScoped<BaseClass>("key", []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &addKeyedScoped(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom<TService, FactoryFcn>(
                std::move(factory), std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds transient service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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
         * @brief Adds keyed transient service descriptor
         * @tparam TService base service type
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addTransient<BaseClass>("key", []() { return std::make_unique<ImplementationClass>(); });
         * @endcode
         */
        template <class TService, class FactoryFcn>
        ServiceCollection &addKeyedTransient(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom<TService, FactoryFcn>(
                std::move(factory), std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds service descriptor
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add(ServiceLifeTimes::Transient, []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &add(const ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom(lifeTime, std::move(factory)));
        }

        /**
         * @brief Adds keyed service descriptor
         * @tparam FactoryFcn factory functor type
         * @param lifeTime service life time Singleton Scoped or Transient
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.add(ServiceLifeTimes::Transient, "key", []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn>
        ServiceCollection &addKeyed(const ServiceLifeTime lifeTime, std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom(lifeTime, std::move(factory),
                                                      std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds singleton service descriptor
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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
         * @brief Adds keyed singleton service descriptor
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addSingleton("key", []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addKeyedSingleton(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom(std::move(factory),
                                                               std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds scoped service descriptor
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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
         * @brief Adds keyed scoped service descriptor
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addScoped("key", []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addKeyedScoped(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom(std::move(factory),
                                                            std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds transient service descriptor
         * @tparam FactoryFcn factory functor type
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
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

        /**
         * @brief Adds keyed transient service descriptor
         * @tparam FactoryFcn factory functor type
         * @param serviceKey service key can be empty to describe default service
         * @param factory service factory functor with this scheme: (Services...) -> std::unique_ptr<TImplementation> |
         * TImplementation, where services are pointers, unique pointers, references, vectors with pointers or unique
         * pointers
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addTransient("key", []() { return std::make_unique<TestClass>(); });
         * @endcode
         */
        template <class FactoryFcn> ServiceCollection &addKeyedTransient(std::string serviceKey, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom(std::move(factory),
                                                               std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds alias service descriptor
         * @tparam TAlias base service type - alias type
         * @tparam TService service type must inherit from TAlias
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addAlias<AliasClass, ServiceClass>();
         * @endcode
         */
        template <class TAlias, class TService> ServiceCollection &addAlias()
        {
            return add(ServiceDescriber::describeAlias<TAlias, TService>());
        }

        /**
         * @brief Adds alias service descriptor
         * @tparam TAlias base service type - alias type
         * @tparam TService service type must inherit from TAlias
         * @param serviceKey service key can be empty for default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addAlias<AliasClass, ServiceClass>("key");
         * @endcode
         */
        template <class TAlias, class TService> ServiceCollection &addAlias(std::string serviceKey)
        {
            return add(ServiceDescriber::describeAlias<TAlias, TService>(
                nullptr, std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds keyed alias service descriptor
         * @tparam TAlias base service type - alias type
         * @tparam TService service type must inherit from TAlias
         * @param serviceAliasKey alias service key can be empty to describe default alias service
         * @param serviceKey service key can be empty for default service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addAlias<AliasClass, ServiceClass>("aliasKey", "key");
         * @endcode
         */
        template <class TAlias, class TService>
        ServiceCollection &addKeyedAlias(std::string serviceAliasKey, std::string serviceKey)
        {
            return add(ServiceDescriber::describeAlias<TAlias, TService>(
                std::make_unique<std::string>(std::move(serviceAliasKey)),
                std::make_unique<std::string>(std::move(serviceKey))));
        }

        /**
         * @brief Adds keyed alias service descriptor
         * @tparam TAlias base service type - alias type
         * @tparam TService service type must inherit from TAlias
         * @param serviceAliasKey alias service key can be empty to describe default alias service
         *
         * Example:
         * @code{.cpp}
         * ServiceCollection{}.addAlias<AliasClass, ServiceClass>("aliasKey");
         * @endcode
         */
        template <class TAlias, class TService> ServiceCollection &addKeyedAlias(std::string serviceAliasKey)
        {
            return add(ServiceDescriber::describeAlias<TAlias, TService>(
                std::make_unique<std::string>(std::move(serviceAliasKey)), nullptr));
        }

        friend bool operator==(const ServiceCollection &lhs, const ServiceCollection &rhs)
        {
            return lhs._serviceDescriptors == rhs._serviceDescriptors;
        }
        friend bool operator!=(const ServiceCollection &lhs, const ServiceCollection &rhs)
        {
            return lhs._serviceDescriptors != rhs._serviceDescriptors;
        }
    };

} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include <SevenBit/DI/Impl/ServiceCollection.hpp>
#endif
