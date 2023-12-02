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
    class EXPORT ServiceList : public OneOrList<IServiceInstance::Ptr>
    {
      private:
        bool _sealed = false;

      public:
        explicit ServiceList(IServiceInstance::Ptr instance);
        explicit ServiceList(size_t size);

        ServiceList(const ServiceList &) = delete;
        ServiceList(ServiceList &&) = default;

        ServiceList &add(IServiceInstance::Ptr &&service);

        void seal();

        [[nodiscard]] bool isSealed() const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceList.hpp"
#endif
