#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceCollection.hpp"

namespace sb::di
{
    class GlobalServices : public ServiceCollection
    {
        GlobalServices() = default;

      public:
        static GlobalServices &instance()
        {
            static GlobalServices services;
            return services;
        }
    };
} // namespace sb::di
