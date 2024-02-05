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
    explicit CircularDependencyUniqueA(std::unique_ptr<CircularDependencyUniqueB> b) {}
};

struct CircularDependencyUniqueB
{
    explicit CircularDependencyUniqueB(std::unique_ptr<CircularDependencyUniqueA> a) {}
};
