#pragma once

#include <type_traits>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Helpers/ServiceCtorArgExtractor.hpp>
#include <SevenBit/DI/Details/Utils/CtorParamsNumber.hpp>
#include <SevenBit/DI/ServiceProvider.hpp>

namespace sb::di::details
{
    template <class T> class CtorInjector
    {
        ServiceProvider &_serviceProvider;

      public:
        explicit CtorInjector(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TWrapper> auto makeUnique()
        {
            return makeUnique<TWrapper>(std::make_index_sequence<ctorParamsNumber<T>()>{});
        };

      private:
        template <class TWrapper, std::size_t... Index> auto makeUnique(std::index_sequence<Index...>)
        {
            return std::make_unique<TWrapper>(ServiceCtorArgExtractor<T>{_serviceProvider, Index}...);
        }
    };

} // namespace sb::di::details
