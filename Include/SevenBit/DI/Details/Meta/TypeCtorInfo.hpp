#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Meta/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Meta/Type.hpp"

namespace sb::di::details
{
    template <class T> class TypeCtorInfo
    {
      public:
        class ArgExtractor
        {
            ServiceProvider *_provider = nullptr;

          public:
            explicit ArgExtractor(ServiceProvider *provider, std::size_t paramNumber = 0) : _provider(provider) {}

            template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S()
            {
                return ServiceGetter<std::remove_cv_t<S>>::get(*_provider);
            }
            template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S &() const
            {
                return _provider->getService<std::remove_cv_t<S>>();
            }
        };

      private:
        template <std::size_t... Ns>
        static constexpr auto paramsNumber(std::size_t) -> decltype(T{ArgExtractor{nullptr, Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <std::size_t... Ns> static constexpr std::size_t paramsNumber(...)
        {

            if constexpr (sizeof...(Ns) > _7BIT_DI_CTOR_PARAMS_LIMIT)
            {
                static_assert(
                    details::notSupportedType<T>,
                    "Proper constructor for specified type was not found, reached maximum constructor params number "
                    "limit, to bump limit define macro _7BIT_DI_CTOR_PARAMS_LIMIT with new value before including lib");
                return 0;
            }
            else
            {
                return paramsNumber<Ns..., sizeof...(Ns)>(0);
            }
        }

      public:
        static constexpr std::size_t parametersNumber = paramsNumber(0);
    };
} // namespace sb::di::details
