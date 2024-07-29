#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include <SevenBit/DI/Details/Helpers/CtorInjector.hpp>
#include <SevenBit/DI/Details/Services/InPlaceService.hpp>

class CtorInjectorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CtorInjectorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CtorInjectorTest() override = default;

    static void TearDownTestSuite() {}
};

struct LotOfParams
{
    LotOfParams(int a, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                int a12)
    {
    }
};

struct LotOfRefs
{
    int &a1;
    int &a2;
    int &a3;
    int &a4;
    int &a5;
    int &a6;
    int &a7;
    int &a8;
    int &a9;
    int &a10;
    int &a11;
};

struct Ambigious
{
    Ambigious(int a, int b) {}
    Ambigious(int a, int b, int d) {}
    Ambigious(int a, int b, int d, int e) {}
};

TEST_F(CtorInjectorTest, ShouldInvokeFuncWithCtorParams)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    sb::di::details::CtorInjector<TestDependencyPtrClass1> invoker{mock};

    auto result = invoker([](auto... params) { return TestDependencyPtrClass1{params...}; });
    EXPECT_EQ(result._test1, test1.getAs<TestDependencyClass>());
}

TEST_F(CtorInjectorTest, ShouldGetProperCtorParamsNumber)
{
    EXPECT_EQ(sb::di::details::CtorInjector<TestComplexClass1>::paramsNumber, 0);
    EXPECT_EQ(sb::di::details::CtorInjector<TestComplexClass2>::paramsNumber, 1);
    EXPECT_EQ(sb::di::details::CtorInjector<TestComplexClass3>::paramsNumber, 2);
    EXPECT_EQ(sb::di::details::CtorInjector<TestComplexClass4>::paramsNumber, 3);
    EXPECT_EQ(sb::di::details::CtorInjector<TestComplexClass5>::paramsNumber, 3);
    EXPECT_EQ(sb::di::details::CtorInjector<LotOfParams>::paramsNumber, 13);
    EXPECT_EQ(sb::di::details::CtorInjector<LotOfRefs>::paramsNumber, 11);
    EXPECT_EQ(sb::di::details::CtorInjector<Ambigious>::paramsNumber, 2);
}
