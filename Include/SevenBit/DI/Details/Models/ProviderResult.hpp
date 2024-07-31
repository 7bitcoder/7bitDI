#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    template <class T> class ProviderResult
    {
      public:
        enum class Error
        {
            None,
            NotRegistered,
            Count
        };

      private:
        T _result;
        Error _error;

      public:
        ProviderResult(T &&result, Error error = Error::None) : _result(std::move(result)), _error(error) {}
        ProviderResult(Error error) : _result({}), _error(error) {}

        bool isOk() const { return _error == Error::None; }

        operator bool() const { return isOk(); };

        const T &getResult() const & { return _result; }
        T &getResult() & { return _result; }
        T &&getResult() && { return std::move(_result); }

        const T &operator()() const & { return getResult(); }
        T &operator()() & { return getResult(); }
        T &&operator()() && { return getResult(); }

        Error getError() const { return _error; }
    };
} // namespace sb::di::details
