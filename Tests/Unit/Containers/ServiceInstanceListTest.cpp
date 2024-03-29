#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ServiceInstanceListTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceInstanceListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceListTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstanceListTest, ShouldAddServices)
{
    TestClass1 test;
    auto implementation = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    auto implementation2 = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    list.add(sb::di::ServiceInstance{std::move(implementation2)});
}

TEST_F(ServiceInstanceListTest, ShouldFailAddNullService)
{
    auto act = [&] { sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{}}; };

    EXPECT_THROW(act(), sb::di::InvalidServiceException);
}

TEST_F(ServiceInstanceListTest, ShouldFailAddInvalidService)
{
    auto act = [&] {
        auto implementation = std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(nullptr);
        sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};
    };

    EXPECT_THROW(act(), sb::di::InvalidServiceException);
}

TEST_F(ServiceInstanceListTest, ShouldReturnProperSize)
{
    auto implementation = std::make_unique<sb::di::details::InPlaceService<TestClass1>>();
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    EXPECT_EQ(list.size(), 1);

    TestClass1 test;
    auto implementation2 = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    list.add(sb::di::ServiceInstance{std::move(implementation2)});

    EXPECT_EQ(list.size(), 2);
}

TEST_F(ServiceInstanceListTest, ShouldReturnEmpty)
{
    auto implementation = std::make_unique<sb::di::details::InPlaceService<TestClass1>>();
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    auto implementation2 = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    list.add(sb::di::ServiceInstance{std::move(implementation2)});

    EXPECT_FALSE(list.empty());
}

TEST_F(ServiceInstanceListTest, ShouldSeal)
{
    auto implementation = std::make_unique<sb::di::details::InPlaceService<TestClass1>>();
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    EXPECT_NO_THROW(list.seal());
    EXPECT_TRUE(list.isSealed());
}

TEST_F(ServiceInstanceListTest, ShouldGetFirst)
{
    auto implementation = std::make_unique<sb::di::details::InPlaceService<TestClass1>>();
    const auto firstServicePtr = implementation.get();
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    auto implementation2 = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    list.add(sb::di::ServiceInstance{std::move(implementation2)});

    EXPECT_EQ(list.first().tryGetImplementation(), firstServicePtr);
}

TEST_F(ServiceInstanceListTest, ShouldGetLast)
{
    auto implementation = std::make_unique<sb::di::details::InPlaceService<TestClass1>>();
    sb::di::details::ServiceInstanceList list{sb::di::ServiceInstance{std::move(implementation)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    auto implementation2 = std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test);
    const auto lastServicePtr = implementation2.get();
    list.add(sb::di::ServiceInstance{std::move(implementation2)});

    EXPECT_EQ(list.last().tryGetImplementation(), lastServicePtr);
}
