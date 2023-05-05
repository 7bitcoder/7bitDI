#pragma once

struct CircularDependencyB;

struct CircularDependencyA
{
    CircularDependencyA(CircularDependencyB *b) {}
};

struct CircularDependencyB
{
    CircularDependencyB(CircularDependencyA *a) {}
};