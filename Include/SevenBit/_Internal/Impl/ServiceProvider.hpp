#pragma once

#include "SevenBit/ServiceProvider.hpp"

namespace sb
{
    INLINE ServiceProvider::ServiceProvider(const IServiceCreatorsProvider *creatorsProvider, ServiceProvider *parent)
        : _creatorsProvider{creatorsProvider}, _parent(parent)
    {
        if (!_parent)
        {
            _singletons = std::make_unique<ServicesContainer>();
        }
    }

    INLINE ServiceProvider ServiceProvider::createScoped() { return ServiceProvider{_creatorsProvider, this}; }

    INLINE void *ServiceProvider::getService(TypeId typeId)
    {
        if (auto list = singletons().getList(typeId))
        {
            return list->at(0);
        }
        if (auto list = scoped().getList(typeId))
        {
            return list->at(0);
        }
        if (typeId == typeid(ServiceProvider))
        {
            return this;
        }
        return createAndRegister(typeId);
    }

    INLINE std::vector<void *> ServiceProvider::getServices(TypeId typeId)
    {
        if (auto list = singletons().getList(typeId); list && list->isSealed())
        {
            return list->getAll();
        }
        if (auto list = scoped().getList(typeId); list && list->isSealed())
        {
            return list->getAll();
        }
        if (typeId == typeid(ServiceProvider))
        {
            return {this};
        }
        return createAndRegisterAll(typeId);
    }

    INLINE void *ServiceProvider::createAndRegister(TypeId typeId)
    {
        if (auto creator = creatorsProvider().getMainCreator(typeId))
        {
            return createAndRegister(*creator);
        }
        return nullptr;
    }

    INLINE std::vector<void *> ServiceProvider::createAndRegisterAll(TypeId typeId)
    {
        if (auto creators = creatorsProvider().getCreators(typeId))
        {
            return createAndRegisterAll(*creators);
        }
        return {};
    }

    INLINE void *ServiceProvider::createAndRegister(const IServiceCreator &creator)
    {
        auto &scope = creator.getServiceScope();
        if (scope.isTransient())
        {
            throw TransientForbidException{creator.getServiceTypeId()};
        }
        auto holder = createHolder(creator);
        auto &container = scope.isSingleton() ? singletons() : scoped();
        return container.addAndGetList(std::move(holder))->at(0);
    }

    INLINE std::vector<void *> ServiceProvider::createAndRegisterAll(const ServiceCreators &creators)
    {
        if (creators.getServicesScope().isTransient())
        {
            throw TransientForbidException{creators.getInterfaceTypeId()};
        }
        auto &container = creators.getServicesScope().isSingleton() ? singletons() : scoped();
        ServiceList *serviceList = container.getList(creators.getInterfaceTypeId());

        if (!serviceList)
        {
            serviceList = container.addAndGetList(createHolder(creators.getMainCreator()));
        }
        serviceList->reserve(creators.size());
        for (auto it = ++creators.begin(); it != creators.end(); ++it) // skip main service
        {
            serviceList->add(createHolder(**it));
        }
        serviceList->seal();
        return serviceList->getAll();
    }

    INLINE IServiceHolder::Ptr ServiceProvider::createHolder(const IServiceCreator &creator)
    {
        auto scopedGuard = _guard.spawnGuard(creator.getServiceTypeId());

        auto holder = creator.create(*this);

        if (holder && holder->isValid())
        {
            return holder;
        }
        throw ConstructionException{creator.getServiceTypeId()};
    }

    INLINE const IServiceCreatorsProvider &ServiceProvider::creatorsProvider()
    {
        if (!_creatorsProvider)
        {
            throw ServiceCreatorProviderNotSet{};
        }
        return *_creatorsProvider;
    }

    INLINE ServicesContainer &ServiceProvider::scoped() { return _scoped; }

    INLINE ServicesContainer &ServiceProvider::singletons()
    {
        if (_singletons)
        {
            return *_singletons;
        }
        if (!_parent)
        {
            throw std::runtime_error("wrong service provider configuration");
        }
        return _parent->singletons();
    }
} // namespace sb
