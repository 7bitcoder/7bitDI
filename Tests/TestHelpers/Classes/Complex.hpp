#pragma once

#include <memory>
#include <string>

#include "SevenBit/DI/ServiceProvider.hpp"

struct ITestComplexClass1
{
    virtual int number() = 0;
    virtual ~ITestComplexClass1() = default;
};

struct TestComplexClass1 : ITestComplexClass1
{
    int number() override { return 1; }
};

struct ITestComplexClass2
{
    virtual ITestComplexClass1 *getOne() = 0;

    virtual int number() = 0;

    virtual ~ITestComplexClass2() = default;
};

struct TestComplexClass2 : ITestComplexClass2
{
    ITestComplexClass1 *_test1;

    explicit TestComplexClass2(ITestComplexClass1 *test1) : _test1(test1) {}

    ITestComplexClass1 *getOne() override { return _test1; }

    int number() override { return 2; }
};

struct ITestComplexClass3
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;

    virtual int number() = 0;
    virtual ~ITestComplexClass3() = default;
};

struct TestComplexClass3 : ITestComplexClass3
{
    ITestComplexClass1 *_test1;
    ITestComplexClass2 *_test2;

    TestComplexClass3(ITestComplexClass1 *test1, ITestComplexClass2 *test2) : _test1(test1), _test2(test2) {}

    ITestComplexClass1 *getOne() override { return _test1; }
    ITestComplexClass2 *getTwo() override { return _test2; }

    int number() override { return 3; }
};

struct ITestComplexClass4
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;
    virtual std::unique_ptr<ITestComplexClass3> &getThree() = 0;

    virtual int number() = 0;

    virtual ~ITestComplexClass4() = default;
};
struct TestComplexClass4 : public ITestComplexClass4
{
    ITestComplexClass1 *_test1;
    ITestComplexClass2 *_test2;
    std::unique_ptr<ITestComplexClass3> _test3;

    TestComplexClass4(ITestComplexClass1 *test1, ITestComplexClass2 *test2, std::unique_ptr<ITestComplexClass3> test3)
        : _test1(test1), _test2(test2), _test3(std::move(test3))
    {
    }

    ITestComplexClass1 *getOne() override { return _test1; }
    ITestComplexClass2 *getTwo() override { return _test2; }
    std::unique_ptr<ITestComplexClass3> &getThree() override { return _test3; }

    int number() override { return 4; }
};

struct ITestComplexClass5
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;
    virtual std::unique_ptr<ITestComplexClass3> makeThree() = 0;

    virtual int number() = 0;
    virtual ~ITestComplexClass5() = default;
};
struct TestComplexClass5 : public ITestComplexClass5
{
    ITestComplexClass1 *_test1;
    ITestComplexClass2 *_test2;
    sb::di::ServiceProvider *_provider;

    TestComplexClass5(ITestComplexClass1 *test1, ITestComplexClass2 *test2, sb::di::ServiceProvider *provider)
        : _test1(test1), _test2(test2), _provider(provider)
    {
    }

    ITestComplexClass1 *getOne() override { return _test1; }
    ITestComplexClass2 *getTwo() override { return _test2; }
    std::unique_ptr<ITestComplexClass3> makeThree() override { return _provider->createService<ITestComplexClass3>(); }

    int number() override { return 5; }
};
