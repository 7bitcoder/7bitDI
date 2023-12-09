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
         * @details Regargind this setting first scoped object are destructured then singletons. If set to true
         * service provider will also destruct services in oposite order to construction.
         */
        bool strongDestructionOrder = false;
        /**
         * @brief Set prebuilds singletons
         * @details If set to true when service provider is constructed it will also construct all singletons
         */
        bool prebuildSingletons = false;

        bool checkServiceGlobalUniqueness = false;
    };
} // namespace sb::di
