#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Meta/TypeCtorInfo.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    template <class T> class CtorInjector
    {
        using TCtorInfo = TypeCtorInfo<T>;
        ServiceProvider &_serviceProvider;

      public:
        explicit CtorInjector(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TWrapper> std::unique_ptr<TWrapper> makeUnique()
        {
            return makeUnique<TWrapper>(std::make_index_sequence<TCtorInfo::parametersNumber>{});
        };

      private:
        template <class TWrapper, std::size_t... ParamNumber>
        std::unique_ptr<TWrapper> makeUnique(std::index_sequence<ParamNumber...>)
        {
            return std::make_unique<TWrapper>(TCtorInfo::ArgExtractor(&_serviceProvider, ParamNumber)...);
        }
    };

} // namespace sb::di::details
