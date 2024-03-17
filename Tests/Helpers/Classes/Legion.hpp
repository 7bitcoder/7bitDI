#pragma once

#include "SevenBit/DI/ServiceCollection.hpp"

template <int ID> class Legion
{
    int data[25] = {0}; // 100b

  public:
    Legion() = default;
};

template <int ID, int Size> void addLegionsRec(sb::di::ServiceCollection &services)
{
    services.addSingleton<Legion<ID>>();
    if (constexpr int Reduced = Size / 2)
    {
        addLegionsRec<ID + Reduced, Reduced>(services);
        addLegionsRec<ID - Reduced, Reduced>(services);
    }
}

template <int Number> void addLegions(sb::di::ServiceCollection &services) { addLegionsRec<Number, Number>(services); }
