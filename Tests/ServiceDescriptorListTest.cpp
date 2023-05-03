#include <gtest/gtest.h>
#include <iostream>

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

TEST_F(ServiceDescriptorListTest, ExampleTest) { EXPECT_TRUE(true); }
