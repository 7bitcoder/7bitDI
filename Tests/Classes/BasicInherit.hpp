#pragma once

#include <string>

struct TestInheritClass1
{
    virtual int number() { return 1; }
};
struct TestInheritClass2 : public TestInheritClass1
{
    int number() { return 2; }
};
struct TestInheritClass3 : public TestInheritClass2
{
    int number() { return 3; }
};
struct TestInheritClass4 : public TestInheritClass3
{
    int number() { return 4; }
};
struct TestInheritClass5 : public TestInheritClass4
{
    int number() { return 5; }
};

struct TestInheritClass6 : public TestInheritClass5
{
    int number() { return 6; }
};

struct TestInheritClass7 final : public TestInheritClass6
{
    int number() { return 7; }
};