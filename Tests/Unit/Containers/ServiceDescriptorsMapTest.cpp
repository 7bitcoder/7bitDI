#include <gtest/gtest.h>

#include "../../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>

class ServiceDescriptorsMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorsMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorsMapTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorsMapTest, ShouldAddDescriptors)
{
    auto act = [&] {
        std::vector<sb::di::ServiceDescriptor> _descriptors;
        sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};

        // todo keep descriptors in stack
        map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
        map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
        const auto descriptor1 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>(
            std::make_unique<std::string>("key"));
        map.add(descriptor1);
        const auto descriptor2 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>(
            std::make_unique<std::string>("key"));
        map.add(descriptor2);
        const auto descriptor3 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>(
            std::make_unique<std::string>("key"));
        map.add(descriptor3);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorsMapTest, ShouldCheckUniqeness)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true};

    map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()); };

    EXPECT_THROW(act(), sb::di::ServiceRegisterException);
}

TEST_F(ServiceDescriptorsMapTest, ShouldCheckUniqenessForKeyed)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true};

    const auto descriptor1 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>(
        std::make_unique<std::string>("key1"));
    map.add(descriptor1);
    const auto descriptor2 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>(
        std::make_unique<std::string>("key3"));
    map.add(descriptor2);
    const auto descriptor3 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>(
        std::make_unique<std::string>("key2"));
    map.add(descriptor3);

    auto act = [&] {
        const auto desc = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>(
            std::make_unique<std::string>("key1"));
        map.add(desc);
    };

    EXPECT_THROW(act(), sb::di::ServiceRegisterException);
}

TEST_F(ServiceDescriptorsMapTest, ShouldCheckUniqenessForAlias)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true};

    map.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass2, TestInheritClass3>());
    const auto desc = sb::di::ServiceDescriber::describeAlias<TestInheritClass2, TestInheritClass3>(
        std::make_unique<std::string>("key"));
    map.add(desc);
    map.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { map.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>()); };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorsMapTest, ShouldNotCheckUniqeness)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), false};

    map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { map.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()); };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorsMapTest, ShouldAddDescriptorsFromVec)
{

    auto act = [&] {
        std::vector<sb::di::ServiceDescriptor> _descriptors;
        _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
        _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
        _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass6>());

        sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorsMapTest, ShouldFailAddServiceDescriptorAlreadyRegistered)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass5>());

    auto act = [&] { sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true}; };

    EXPECT_THROW(act(), sb::di::ServiceRegisterException);
}

TEST_F(ServiceDescriptorsMapTest, ShouldFailAddKeyedServiceDescriptorAlreadyRegistered)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    const auto descriptor1 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>(
        std::make_unique<std::string>("key"));
    _descriptors.emplace_back(descriptor1);
    const auto descriptor2 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>(
        std::make_unique<std::string>("key"));
    _descriptors.emplace_back(descriptor2);
    const auto descriptor3 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>(
        std::make_unique<std::string>("key"));
    _descriptors.emplace_back(descriptor3);
    const auto descriptor4 = sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass5>(
        std::make_unique<std::string>("key"));
    _descriptors.emplace_back(descriptor4);

    auto act = [&] { sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true}; };

    EXPECT_THROW(act(), sb::di::ServiceRegisterException);
}

TEST_F(ServiceDescriptorsMapTest, ShouldSealDescriptors)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass6>());

    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};
    auto act = [&] { map.seal(); };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorsMapTest, ShouldFindDescriptorList)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass6>());
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass3, TestInheritClass7>(
        std::make_unique<std::string>("key"));
    _descriptors.emplace_back(descriptor);

    sb::di::details::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};

    auto first = map.findDescriptors(sb::di::details::ServiceId{typeid(TestInheritClass2)});
    EXPECT_TRUE(first);
    EXPECT_EQ(first->size(), 2);
    EXPECT_EQ(first->first().getServiceTypeId(), typeid(TestInheritClass2));
    EXPECT_EQ(first->first().getImplementationTypeId(), typeid(TestInheritClass3));
    EXPECT_EQ(first->first().getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_FALSE(first->first().getServiceKey());
    EXPECT_FALSE(first->first().getImplementationKey());
    EXPECT_EQ(first->last().getServiceTypeId(), typeid(TestInheritClass2));
    EXPECT_EQ(first->last().getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_EQ(first->last().getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_FALSE(first->last().getServiceKey());
    EXPECT_FALSE(first->last().getImplementationKey());

    auto second = map.findDescriptors(sb::di::details::ServiceId{typeid(TestInheritClass1)});
    EXPECT_TRUE(second);
    EXPECT_EQ(second->size(), 2);
    EXPECT_EQ(second->first().getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(second->first().getImplementationTypeId(), typeid(TestInheritClass4));
    EXPECT_EQ(second->first().getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_FALSE(second->first().getServiceKey());
    EXPECT_FALSE(second->first().getImplementationKey());
    EXPECT_EQ(second->last().getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(second->last().getImplementationTypeId(), typeid(TestInheritClass6));
    EXPECT_EQ(second->last().getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_FALSE(second->last().getServiceKey());
    EXPECT_FALSE(second->last().getImplementationKey());

    auto third = map.findDescriptors(sb::di::details::ServiceId{typeid(TestInheritClass3), "key"});
    EXPECT_TRUE(third);
    EXPECT_EQ(third->size(), 1);
    EXPECT_EQ(third->first().getServiceTypeId(), typeid(TestInheritClass3));
    EXPECT_EQ(third->first().getImplementationTypeId(), typeid(TestInheritClass7));
    EXPECT_EQ(third->first().getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_TRUE(third->first().getServiceKey());
    EXPECT_FALSE(third->first().getImplementationKey());
    EXPECT_EQ(*third->first().getServiceKey(), "key");

    EXPECT_FALSE(map.findDescriptors(sb::di::details::ServiceId{typeid(TestInheritClass4)}));
}
