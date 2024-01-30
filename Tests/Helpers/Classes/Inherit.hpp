#pragma once

struct TestInheritClass1
{
    virtual int number() { return 1; }
};
struct TestInheritClass2 : TestInheritClass1
{
    int number() override { return 2; }
};
struct TestInheritClass3 : TestInheritClass2
{
    int number() override { return 3; }
};
struct TestInheritClass4 : TestInheritClass3
{
    int number() override { return 4; }
};
struct TestInheritClass5 : TestInheritClass4
{
    int number() override { return 5; }
};

struct TestInheritClass6 : TestInheritClass5
{
    int number() override { return 6; }
};
struct TestInheritClass7 : TestInheritClass6
{
    int number() override { return 7; }
};
struct TestInheritClass8 final : TestInheritClass7
{
    int number() override { return 8; }
};
