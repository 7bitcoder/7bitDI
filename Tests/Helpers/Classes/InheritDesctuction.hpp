#pragma once

struct TestInheritDestrClass1
{
    virtual ~TestInheritDestrClass1() = default;
};
struct TestInheritDestrClass2 : TestInheritDestrClass1
{
};
struct TestInheritDestrClass3 : TestInheritDestrClass2
{
};
struct TestInheritDestrClass4 : TestInheritDestrClass3
{
};
template <class Fcn> struct TestInheritDestrClass5 final : TestInheritDestrClass4
{
    Fcn _fcn;
    TestInheritDestrClass5(Fcn fcn) : _fcn(fcn) {}

    ~TestInheritDestrClass5() { _fcn(); }
};
