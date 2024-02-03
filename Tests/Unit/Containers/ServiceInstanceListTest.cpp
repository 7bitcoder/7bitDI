#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

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
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::ExternalService{&test}};
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::services::ExternalService{&test}};
    list.add(sb::di::ServiceInstance{std::move(instance2)});
}

TEST_F(ServiceInstanceListTest, ShouldNotFailAddNullService)
{
    auto act = [&]() { sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{}}; };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceInstanceListTest, ShouldFailAddInvalidService)
{
    auto act = [&]() {
        sb::di::IServiceInstance::Ptr service{new sb::di::details::services::UniquePtrService<TestClass1>{nullptr}};
        sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};
    };

    EXPECT_THROW((act()), sb::di::InvalidServiceException);
}

TEST_F(ServiceInstanceListTest, ShouldReturnProperSize)
{
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::InPlaceService<TestClass1>{}};
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    EXPECT_EQ(list.size(), 1);

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::services::ExternalService{&test}};
    list.add(sb::di::ServiceInstance{std::move(instance2)});

    EXPECT_EQ(list.size(), 2);
}

TEST_F(ServiceInstanceListTest, ShouldReturnEmpty)
{
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::InPlaceService<TestClass1>{}};
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::services::ExternalService{&test}};
    list.add(sb::di::ServiceInstance{std::move(instance2)});

    EXPECT_FALSE(list.empty());
}

TEST_F(ServiceInstanceListTest, ShouldSeal)
{
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::InPlaceService<TestClass1>{}};
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    EXPECT_NO_THROW(list.seal());
    EXPECT_TRUE(list.isSealed());
}

TEST_F(ServiceInstanceListTest, ShouldGetFirst)
{
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::InPlaceService<TestClass1>{}};
    const auto firstServicePtr = service.get();
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::services::ExternalService{&test}};
    list.add(sb::di::ServiceInstance{std::move(instance2)});

    EXPECT_EQ(&list.first().getImplementation(), firstServicePtr);
}

TEST_F(ServiceInstanceListTest, ShouldGetLast)
{
    sb::di::IServiceInstance::Ptr service{new sb::di::details::services::InPlaceService<TestClass1>{}};
    sb::di::details::containers::ServiceInstanceList list{sb::di::ServiceInstance{std::move(service)}};

    EXPECT_FALSE(list.empty());

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::services::ExternalService{&test}};
    const auto lastServicePtr = instance2.get();
    list.add(sb::di::ServiceInstance{std::move(instance2)});

    EXPECT_EQ(&list.last().getImplementation(), lastServicePtr);
}
