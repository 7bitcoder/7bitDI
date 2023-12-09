#include <gtest/gtest.h>

class Template : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    Template() {}

    void SetUp() override {}

    void TearDown() override {}

    ~Template() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(Template, ExampleTest) { EXPECT_TRUE(true); }
