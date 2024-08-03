#pragma once
#include <memory>

struct CircularDependencyB;

struct CircularDependencyA
{
    explicit CircularDependencyA(CircularDependencyB *b) {}
};

struct CircularDependencyB
{
    explicit CircularDependencyB(CircularDependencyA *a) {}
};

struct CircularDependencyUniqueB;

struct CircularDependencyUniqueA
{
    explicit CircularDependencyUniqueA(std::unique_ptr<CircularDependencyUniqueB>);

    ~CircularDependencyUniqueA();
};

struct CircularDependencyUniqueB
{
    explicit CircularDependencyUniqueB(std::unique_ptr<CircularDependencyUniqueA>);

    ~CircularDependencyUniqueB();
};

inline CircularDependencyUniqueA::CircularDependencyUniqueA(std::unique_ptr<CircularDependencyUniqueB> b) {}
inline CircularDependencyUniqueA::~CircularDependencyUniqueA() = default;
inline CircularDependencyUniqueB::CircularDependencyUniqueB(std::unique_ptr<CircularDependencyUniqueA> a) {}
inline CircularDependencyUniqueB::~CircularDependencyUniqueB() = default;
