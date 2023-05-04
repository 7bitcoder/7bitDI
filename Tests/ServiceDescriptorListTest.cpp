#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ServiceCtorFactory.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"

class ServiceDescriptorListTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorListTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorListTest, ShouldAddServiceDescriptors)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory3)});
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorAlreadyRegistered)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    auto act = [&]() { list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory3)}); };

    EXPECT_THROW((act()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorLifeTimeMismatch)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    auto act = [&]() { list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::singleton(), std::move(factory3)}); };

    EXPECT_THROW((act()), sb::ServiceLifeTimeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorBaseTypeMismatch)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    auto act = [&]() { list.add({typeid(TestInheritClass2), sb::ServiceLifeTime::scoped(), std::move(factory3)}); };

    EXPECT_THROW((act()), sb::ServiceBaseTypeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldContainDescriptor)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory3)});

    EXPECT_TRUE(list.contains(typeid(TestInheritClass5)));
    EXPECT_TRUE(list.contains(typeid(TestInheritClass4)));
    EXPECT_TRUE(list.contains(typeid(TestInheritClass3)));
    EXPECT_FALSE(list.contains(typeid(TestInheritClass2)));
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperSize)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory3)});

    EXPECT_EQ(list.size(), 3);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperEmpty)
{
    sb::internal::ServiceDescriptorList list;

    auto factory = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass3>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory)});

    auto factory2 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass4>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory2)});

    auto factory3 = sb::IServiceFactory::Ptr{new sb::internal::ServiceCtorFactory<TestInheritClass5>()};
    list.add({typeid(TestInheritClass1), sb::ServiceLifeTime::scoped(), std::move(factory3)});

    EXPECT_FALSE(list.empty());
}