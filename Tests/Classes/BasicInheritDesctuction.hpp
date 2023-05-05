#pragma once

struct TestInheritDestrClass1
{
    virtual ~TestInheritDestrClass1() = default;
};
struct TestInheritDestrClass2 : public TestInheritDestrClass1
{
};
struct TestInheritDestrClass3 : public TestInheritDestrClass2
{
};
struct TestInheritDestrClass4 : public TestInheritDestrClass3
{
};
template <class Fcn> struct TestInheritDestrClass5 final : public TestInheritDestrClass4
{
    Fcn _fcn;
    TestInheritDestrClass5(Fcn fcn) : _fcn(fcn) {}

    ~TestInheritDestrClass5() { _fcn(); }
};