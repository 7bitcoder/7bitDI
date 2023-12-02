#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/OneOrList.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstanceList : public OneOrList<IServiceInstance::Ptr>
    {
      private:
        bool _sealed = false;

      public:
        explicit ServiceInstanceList(IServiceInstance::Ptr instance);
        explicit ServiceInstanceList(size_t size);

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        ServiceInstanceList &add(IServiceInstance::Ptr &&service);

        void seal();

        [[nodiscard]] bool isSealed() const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceInstanceList.hpp"
#endif
