#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/ServiceCtorFactory.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"

class ServiceListTest : public ::testing::Test
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
    sb::internal::ServiceList list;

    sb::IServiceInstance::Ptr instance{new sb::internal::ExternalService{&test}};
    list.add(std::move(instance));

    sb::IServiceInstance::Ptr instance2{new sb::internal::ExternalService{&test}};
    list.add(std::move(instance2));
}

TEST_F(ServiceListTest, ShouldFailAddNullService)
{
    TestClass1 test;
    sb::internal::ServiceList list;

    sb::IServiceInstance::Ptr instance{nullptr};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::NullPointnerException);
}

TEST_F(ServiceListTest, ShouldFailAddInvalidService)
{
    TestClass1 test;
    sb::internal::ServiceList list;

    sb::IServiceInstance::Ptr instance{new sb::internal::ServiceOwner<TestClass1>{nullptr}};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::NullPointnerException);
}
TEST_F(ServiceListTest, ShouldGetAllServices)
{
    sb::internal::ServiceList list;

    TestClass1 test;
    sb::IServiceInstance::Ptr instance{new sb::internal::ExternalService{&test}};
    list.add(std::move(instance));

    TestClass1 test2;
    sb::IServiceInstance::Ptr instance2{new sb::internal::ExternalService{&test2}};
    list.add(std::move(instance2));

    auto services = list.getAllServices();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0], &test2);
    EXPECT_EQ(services[1], &test);
}