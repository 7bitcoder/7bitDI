#pragma once

#include <memory>
#include <string>

#include "SevenBit/DI/ServiceProvider.hpp"

struct TestDependencyClass1
{
    int number() { return 1; }
};

struct TestDependencyClass2
{
    TestDependencyClass1 *_test1;

    TestDependencyClass2(TestDependencyClass1 *test1) : _test1(test1) {}

    int number() { return 2; }
};

struct TestDependencyClass3
{
    TestDependencyClass1 *_test1;
    TestDependencyClass2 *_test2;

    TestDependencyClass3(TestDependencyClass1 *test1, TestDependencyClass2 *test2) : _test1(test1), _test2(test2) {}

    int number() { return 3; }
};

struct ITestComplexClass1
{
    virtual int number() = 0;
    virtual ~ITestComplexClass1() {}
};

struct TestComplexClass1 : public ITestComplexClass1
{
    int number() { return 1; }
};

struct ITestComplexClass2
{
    virtual ITestComplexClass1 *getOne() = 0;

    virtual int number() = 0;

    virtual ~ITestComplexClass2() {}
};
struct TestComplexClass2 : public ITestComplexClass2
{
    ITestComplexClass1 *_test1;

    TestComplexClass2(ITestComplexClass1 *test1) : _test1(test1) {}

    ITestComplexClass1 *getOne() { return _test1; }

    int number() { return 2; }
};

struct ITestComplexClass3
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;

    virtual int number() = 0;
    virtual ~ITestComplexClass3() {}
};
struct TestComplexClass3 : public ITestComplexClass3
{
    ITestComplexClass1 *_test1;
    ITestComplexClass2 *_test2;

    TestComplexClass3(ITestComplexClass1 *test1, ITestComplexClass2 *test2) : _test1(test1), _test2(test2) {}

    ITestComplexClass1 *getOne() { return _test1; }
    ITestComplexClass2 *getTwo() { return _test2; }

    int number() { return 3; }
};

struct ITestComplexClass4
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;
    virtual std::unique_ptr<ITestComplexClass3> &getThree() = 0;

    virtual int number() = 0;

    virtual ~ITestComplexClass4() {}
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

    ITestComplexClass1 *getOne() { return _test1; }
    ITestComplexClass2 *getTwo() { return _test2; }
    std::unique_ptr<ITestComplexClass3> &getThree() { return _test3; }

    int number() { return 4; }
};

struct ITestComplexClass5
{
    virtual ITestComplexClass1 *getOne() = 0;
    virtual ITestComplexClass2 *getTwo() = 0;
    virtual std::unique_ptr<ITestComplexClass3> makeThree() = 0;

    virtual int number() = 0;
    virtual ~ITestComplexClass5() {}
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

    ITestComplexClass1 *getOne() { return _test1; }
    ITestComplexClass2 *getTwo() { return _test2; }
    std::unique_ptr<ITestComplexClass3> makeThree() { return _provider->createService<ITestComplexClass3>(); }

    int number() { return 5; }
};
