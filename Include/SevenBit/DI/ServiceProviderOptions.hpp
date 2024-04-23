#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

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
         * @details If set to true service provider will check if service implementation was already registered and
         * throw exception in this case
         */
        bool checkServiceGlobalUniqueness = true;

        /**
         * @brief Set service search strategy
         * @details If set to true provider will search for service in singleton container first then in scoped
         */
        bool searchInSigletonsFirst = true;
    };
} // namespace sb::di
