#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstancesResolver
    {
        ServiceInstanceCreator &_creator;
        const ServiceDescriptorList &_descriptors;

      public:
        ServiceInstancesResolver(ServiceInstanceCreator &creator, const ServiceDescriptorList &descriptors);

        [[nodiscard]] ServiceInstance create() const;
        [[nodiscard]] ServiceInstanceList createOne() const;
        [[nodiscard]] ServiceInstanceList createAll() const;
        ServiceInstanceList &createRest(ServiceInstanceList &instances) const;

        [[nodiscard]] ServiceInstance createInPlace() const;
        [[nodiscard]] ServiceInstanceList createOneInPlace() const;
        [[nodiscard]] ServiceInstanceList createAllInPlace() const;
        ServiceInstanceList &createRestInPlace(ServiceInstanceList &instances) const;

      private:
        [[nodiscard]] ServiceInstance create(bool inPlaceRequest) const;
        [[nodiscard]] ServiceInstanceList createOne(bool inPlaceRequest) const;
        [[nodiscard]] ServiceInstanceList createAll(bool inPlaceRequest) const;
        ServiceInstanceList &createRest(ServiceInstanceList &instances, bool inPlaceRequest) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesResolver.hpp"
#endif
