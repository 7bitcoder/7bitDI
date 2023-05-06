#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/Details/ExternalService.hpp"
#include "SevenBit/Details/ServiceCtorFactory.hpp"
#include "SevenBit/Details/ServiceDescriptorList.hpp"
#include "SevenBit/Details/ServiceList.hpp"
#include "SevenBit/Details/ServiceOwner.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/ServiceLifeTime.hpp"

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
    sb::details::ServiceList list;

    sb::IServiceInstance::Ptr instance{new sb::details::ExternalService{&test}};
    list.add(std::move(instance));

    sb::IServiceInstance::Ptr instance2{new sb::details::ExternalService{&test}};
    list.add(std::move(instance2));
}

TEST_F(ServiceListTest, ShouldFailAddNullService)
{
    sb::details::ServiceList list;

    sb::IServiceInstance::Ptr instance{nullptr};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::NullPointnerException);
}

TEST_F(ServiceListTest, ShouldFailAddInvalidService)
{
    sb::details::ServiceList list;

    sb::IServiceInstance::Ptr instance{new sb::details::ServiceOwner<TestClass1>{nullptr}};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::NullPointnerException);
}
TEST_F(ServiceListTest, ShouldGetAllServices)
{
    sb::details::ServiceList list;

    TestClass1 test;
    sb::IServiceInstance::Ptr instance{new sb::details::ExternalService{&test}};
    list.add(std::move(instance));

    TestClass1 test2;
    sb::IServiceInstance::Ptr instance2{new sb::details::ExternalService{&test2}};
    list.add(std::move(instance2));

    auto services = list.getAllServices();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->get(), &test2);
    EXPECT_EQ(services[1]->get(), &test);
}