#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceAliasesCreator.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Details/Utils/RequireDescriptor.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"

namespace sb::di::details
{
    INLINE ServiceInstance ServiceAliasesCreator::tryCreate(const ServiceDescriptor &descriptor,
                                                            const ServiceInstance *original) const
    {
        return original ? create(descriptor, *original) : ServiceInstance{};
    }

    INLINE ServiceInstance ServiceAliasesCreator::tryMap(const ServiceDescriptor &descriptor,
                                                         ServiceInstance &&original) const
    {
        original.addCastOffset(descriptor.getCastOffset());
        return std::move(original);
    }

    INLINE void ServiceAliasesCreator::tryCreateAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                                                    const ServiceInstanceList *originals) const
    {
        if (originals)
        {
            instances.reserve(instances.size() + originals->size());
            originals->getInnerList().forEach(
                [&](const ServiceInstance &instance) { instances.add(create(descriptor, instance)); });
        }
    }

    INLINE void ServiceAliasesCreator::tryCreateRest(ServiceInstanceList &instances,
                                                     const ServiceDescriptor &descriptor,
                                                     const ServiceInstanceList *originals,
                                                     ServiceInstance &&first) const
    {
        if (originals)
        {
            const auto size = originals->size();
            instances.reserve(instances.size() + size);
            originals->getInnerList().forEach([&](const ServiceInstance &instance, const std::size_t index) {
                if (index < size - 1) // skip last
                {
                    instances.add(create(descriptor, instance));
                }
            });
            instances.add(std::move(first));
        }
    }

    INLINE void ServiceAliasesCreator::tryMapAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                                                 OneOrList<ServiceInstance> &&originals) const
    {
        if (originals)
        {
            if (descriptor.getCastOffset())
            {
                originals.forEach(
                    [&](ServiceInstance &instance) { instance.addCastOffset(descriptor.getCastOffset()); });
            }
            instances.add(std::move(originals));
        }
    }

    INLINE ServiceInstance ServiceAliasesCreator::create(const ServiceDescriptor &descriptor,
                                                         const ServiceInstance &original) const
    {
        RequireDescriptor::alias(descriptor);
        RequireInstance::valid(original);
        auto implementationType = descriptor.getImplementationTypeId();

        auto implementation = std::make_unique<AliasService>(original.getAs<void>(), implementationType);
        return ServiceInstance{std::move(implementation), descriptor.getCastOffset()};
    }
} // namespace sb::di::details
