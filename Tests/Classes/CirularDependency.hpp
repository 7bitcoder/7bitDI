#pragma once
#include <memory>

struct CircularDependencyB;

struct CircularDependencyA
{
    CircularDependencyA(CircularDependencyB *b) {}
};

struct CircularDependencyB
{
    CircularDependencyB(CircularDependencyA *a) {}
};

struct CircularDependencyUniqueB;

struct CircularDependencyUniqueA
{
    CircularDependencyUniqueA(std::unique_ptr<CircularDependencyUniqueB> b) {}
};

struct CircularDependencyUniqueB
{
    CircularDependencyUniqueB(std::unique_ptr<CircularDependencyUniqueA> a) {}
};