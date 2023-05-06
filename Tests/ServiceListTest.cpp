#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/ServiceList.hpp"
#include "SevenBit/DI/Details/ServiceOwner.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

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
    sb::di::details::ServiceList list;

    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    list.add(std::move(instance));

    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::ExternalService{&test}};
    list.add(std::move(instance2));
}

TEST_F(ServiceListTest, ShouldFailAddNullService)
{
    sb::di::details::ServiceList list;

    sb::di::IServiceInstance::Ptr instance{nullptr};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::di::NullPointnerException);
}

TEST_F(ServiceListTest, ShouldFailAddInvalidService)
{
    sb::di::details::ServiceList list;

    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ServiceOwner<TestClass1>{nullptr}};
    auto act = [&]() { list.add(std::move(instance)); };

    EXPECT_THROW((act()), sb::di::NullPointnerException);
}
TEST_F(ServiceListTest, ShouldGetAllServices)
{
    sb::di::details::ServiceList list;

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    list.add(std::move(instance));

    TestClass1 test2;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::ExternalService{&test2}};
    list.add(std::move(instance2));

    auto services = list.getAllServices();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->get(), &test2);
    EXPECT_EQ(services[1]->get(), &test);
}