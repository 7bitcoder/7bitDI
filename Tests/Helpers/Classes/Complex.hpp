#pragma once

#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <SevenBit/DI/ServiceCollection.hpp>
#include <SevenBit/DI/ServiceProvider.hpp>

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

struct ITestComplexClass6
{
    virtual ITestComplexClass1 &getOne() = 0;
    virtual ITestComplexClass2 &getTwo() = 0;
    virtual int *getNonExisting() = 0;
    virtual std::unique_ptr<ITestComplexClass3> makeThree() = 0;

    virtual int number() = 0;
    virtual ~ITestComplexClass6() = default;
};
struct TestComplexClass6 : public ITestComplexClass6
{
    ITestComplexClass1 &_test1;
    ITestComplexClass2 &_test2;
    sb::di::ServiceProvider &_provider;
    int *_nonExisting;

    TestComplexClass6(ITestComplexClass1 &test1, ITestComplexClass2 &test2, sb::di::ServiceProvider &provider,
                      int *nonExisting)
        : _test1(test1), _test2(test2), _provider(provider), _nonExisting(nonExisting)
    {
    }

    ITestComplexClass1 &getOne() override { return _test1; }
    ITestComplexClass2 &getTwo() override { return _test2; }
    std::unique_ptr<ITestComplexClass3> makeThree() override { return _provider.createService<ITestComplexClass3>(); }
    int *getNonExisting() override { return _nonExisting; };

    int number() override { return 6; }
};

template <size_t ID> struct TestNested
{
    TestNested<ID - 1> &_nested;

    explicit TestNested(TestNested<ID - 1> &nested) : _nested(nested) {}

    static void addAllRecurse(sb::di::ServiceCollection &collection)
    {
        collection.addSingleton<TestNested<ID>>();
        TestNested<ID - 1>::addAllRecurse(collection);
    }

    void checkRecurse(sb::di::ServiceProvider &provider)
    {
        EXPECT_EQ(&_nested, &provider.getService<TestNested<ID - 1>>());
        _nested.checkRecurse(provider);
    }
};

template <> struct TestNested<0>
{
    TestNested() = default;

    static void addAllRecurse(sb::di::ServiceCollection &collection) { collection.addSingleton<TestNested<0>>(); }

    void checkRecurse(sb::di::ServiceProvider &provider) {}
};
