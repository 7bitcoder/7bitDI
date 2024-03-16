#pragma once

#include "SevenBit/DI/ServiceCollection.hpp"

template <int ID> class Legion
{
    int data[25] = {0}; // 100b

  public:
    Legion() = default;
};

template <int ID, int Size> void addLegions(sb::di::ServiceCollection &services)
{
    services.addSingleton<Legion<ID>>();
    if (Size / 2)
    {
        addLegions<ID + Size / 2, Size / 2>(services);
        addLegions<ID - Size / 2, Size / 2>(services);
    }
}
