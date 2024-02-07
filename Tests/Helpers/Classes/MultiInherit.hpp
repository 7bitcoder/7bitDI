#pragma once

struct TestMultiInheritClass1
{
    int arr[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 1;
    virtual int first() { return 1; }

    virtual ~TestMultiInheritClass1() = default;
};
struct TestMultiInherit2Class1
{
    int arr[30] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 2;
    virtual int second() { return 11; }

    virtual ~TestMultiInherit2Class1() = default;
};
struct TestMultiInheritClass2 : TestMultiInheritClass1, TestMultiInherit2Class1
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 3;
    int first() override { return 2; }
    int second() override { return 22; }
};
struct TestMultiInherit3Class1
{
    int arr[40] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 4;
    virtual int third() { return 111; }

    virtual ~TestMultiInherit3Class1() = default;
};

struct TestMultiInheritClass3 : TestMultiInheritClass2, TestMultiInherit3Class1
{
    int arr[50] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 5;
    int first() override { return 3; }
    int second() override { return 33; }
    int third() override { return 333; }
};

struct TestMultiInherit4Class4
{
    int arr[80] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 6;
    virtual int fourth() { return 1111; }

    virtual ~TestMultiInherit4Class4() = default;
};

struct TestMultiInheritClass4 : TestMultiInheritClass3, TestMultiInherit4Class4
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int end = 7;
    int first() override { return 4; }
    int second() override { return 44; }
    int third() override { return 444; }
    int fourth() override { return 4444; }
};
