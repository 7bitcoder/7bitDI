#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/ServiceOwner.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

class ServiceListTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceListTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceListTest, ShouldAddServices)
{
    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    sb::di::details::ServiceInstanceList list{std::move(instance)};

    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::ExternalService{&test}};
    list.add(std::move(instance2));
}

TEST_F(ServiceListTest, ShouldFailAddNullService)
{
    auto act = [&]() { sb::di::details::ServiceInstanceList list{nullptr}; };

    EXPECT_THROW((act()), sb::di::NullPointnerException);
}

TEST_F(ServiceListTest, ShouldFailAddInvalidService)
{
    auto act = [&]() {
        sb::di::IServiceInstance::Ptr instance{new sb::di::details::ServiceOwner<TestClass1>{nullptr}};
        sb::di::details::ServiceInstanceList list{std::move(instance)};
    };

    EXPECT_THROW((act()), sb::di::InvalidServiceException);
}
