#pragma once

#include <memory>
#include <string>

#include "SevenBit/DI/ServiceProvider.hpp"

struct TestDependencyClass
{
};

struct TestDependencyInPlaceClass
{
    TestDependencyClass _test1;

    explicit TestDependencyInPlaceClass(TestDependencyClass test1) : _test1(test1) {}
};

struct TestDependencyPtrClass1
{
    TestDependencyClass *_test1;

    explicit TestDependencyPtrClass1(TestDependencyClass *test1) : _test1(test1) {}
};

struct TestDependencyPtrClass2
{
    const TestDependencyClass *_test1;

    explicit TestDependencyPtrClass2(const TestDependencyClass *test1) : _test1(test1) {}
};

struct TestDependencyPtrClass3
{
    const TestDependencyClass *const _test1;

    explicit TestDependencyPtrClass3(const TestDependencyClass *const test1) : _test1(test1) {}
};

struct TestDependencyPtrClass4
{
    TestDependencyClass *const _test1;

    explicit TestDependencyPtrClass4(TestDependencyClass *const test1) : _test1(test1) {}
};

struct TestDependencyRefClass1
{
    TestDependencyClass &_test1;

    explicit TestDependencyRefClass1(TestDependencyClass &test1) : _test1(test1) {}
};

struct TestDependencyRefClass2
{
    const TestDependencyClass &_test1;

    explicit TestDependencyRefClass2(const TestDependencyClass &test1) : _test1(test1) {}
};

struct TestDependencyUniquePtrClass1
{
    std::unique_ptr<TestDependencyClass> _test1;

    explicit TestDependencyUniquePtrClass1(std::unique_ptr<TestDependencyClass> test1) : _test1(std::move(test1)) {}
};

struct TestDependencyUniquePtrClass2
{
    const std::unique_ptr<TestDependencyClass> _test1;

    explicit TestDependencyUniquePtrClass2(const std::unique_ptr<TestDependencyClass> test1) {}
};

struct TestDependencyVecClass1
{
    std::vector<TestDependencyClass *> _test1;

    explicit TestDependencyVecClass1(std::vector<TestDependencyClass *> test1) : _test1(std::move(test1)) {}
};

struct TestDependencyVecClass2
{
    const std::vector<TestDependencyClass *> _test1;

    explicit TestDependencyVecClass2(const std::vector<TestDependencyClass *> test1) : _test1(test1) {}
};

struct TestDependencyVecClass3
{
    std::vector<std::unique_ptr<TestDependencyClass>> _test1;

    explicit TestDependencyVecClass3(std::vector<std::unique_ptr<TestDependencyClass>> test1) : _test1(std::move(test1))
    {
    }
};

struct TestDependencyVecClass4
{
    const std::vector<std::unique_ptr<TestDependencyClass>> _test1;

    explicit TestDependencyVecClass4(const std::vector<std::unique_ptr<TestDependencyClass>> test1) {}
};
