#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceCollection.hpp"

namespace sb::di
{
    class GlobalServices : public ServiceCollection
    {
        GlobalServices() = default;

      public:
        GlobalServices(const GlobalServices &) = delete;
        GlobalServices(GlobalServices &&) noexcept = delete;
        GlobalServices &operator=(const GlobalServices &) = delete;
        GlobalServices &operator=(GlobalServices &&) noexcept = delete;

        static GlobalServices &instance()
        {
            static GlobalServices services;
            return services;
        }
    };
} // namespace sb::di
