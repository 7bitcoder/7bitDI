#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    /**
     * @brief Options object used to configure service provider
     */
    struct ServiceProviderOptions
    {
        /**
         * @brief Set strong service destruction order
         * @details If set to true service provider will destruct services in opposite order to construction.
         */
        bool strongDestructionOrder = false;
        /**
         * @brief Set prebuilds singletons
         * @details If set to true when service provider is constructed it will also construct all singletons
         */
        bool prebuildSingletons = false;

        /**
         * @brief Set global uniqueness check
         * @details If set to true service provider will check if service was already registered
         */
        bool checkServiceGlobalUniqueness = true;
    };
} // namespace sb::di
