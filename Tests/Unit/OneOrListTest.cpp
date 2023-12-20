#include <gtest/gtest.h>

#include "SevenBit/DI/OneOrList.hpp"

class OneOrListTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    OneOrListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~OneOrListTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(OneOrListTest, ShouldCreateSingle)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_FALSE(list.isList());
    EXPECT_EQ(list.first(), 2);
}

TEST_F(OneOrListTest, ShouldCreateList)
{
    sb::di::OneOrList<int> list(2);
    list.reserve(2);

    EXPECT_TRUE(list.isList());
    EXPECT_EQ(list.size(), 1);
}

TEST_F(OneOrListTest, ShouldMove)
{
    sb::di::OneOrList<int> list{2};

    auto newList = std::move(list);

    EXPECT_FALSE(list.isList());
    EXPECT_EQ(list.first(), 2);
    EXPECT_FALSE(newList.isList());
    EXPECT_EQ(newList.first(), 2);
}

TEST_F(OneOrListTest, ShouldAdd)
{
    sb::di::OneOrList<int> list{2};

    list.add(3);
    list.add(4);

    EXPECT_TRUE(list.isList());
    EXPECT_EQ(list[0], 2);
    EXPECT_EQ(list[1], 3);
    EXPECT_EQ(list[2], 4);
}

TEST_F(OneOrListTest, ShouldGetFirst)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_EQ(list.first(), 2);

    list.add(3);
    list.add(4);

    EXPECT_TRUE(list.isList());
    EXPECT_EQ(list.first(), 2);
}

TEST_F(OneOrListTest, ShouldGetLast)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_EQ(list.last(), 2);

    list.add(3);
    list.add(4);

    EXPECT_TRUE(list.isList());
    EXPECT_EQ(list.last(), 4);
}

TEST_F(OneOrListTest, ShouldGetIndexed)
{
    sb::di::OneOrList<int> list{2};

    list.add(3);
    list.add(4);
    list.add(4);

    EXPECT_EQ(list[0], 2);
    EXPECT_EQ(list[1], 3);
    EXPECT_EQ(list[2], 4);
    EXPECT_EQ(list[3], 4);
}

TEST_F(OneOrListTest, ShouldGetSize)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_EQ(list.size(), 1);

    list.add(3);
    list.add(4);

    EXPECT_EQ(list.size(), 3);

    list.add(4);
    EXPECT_EQ(list.size(), 4);
}

TEST_F(OneOrListTest, ShouldGetEmpty)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_FALSE(list.empty());

    list.add(3);
    list.add(4);

    EXPECT_FALSE(list.empty());

    list.add(4);
    EXPECT_FALSE(list.empty());
}

TEST_F(OneOrListTest, ShouldGetIsList)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_FALSE(list.isList());

    list.add(3);
    EXPECT_TRUE(list.isList());
    list.add(4);
    list.add(4);
    EXPECT_TRUE(list.isList());
}

TEST_F(OneOrListTest, ShouldGetAsList)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_THROW(list.getAsList(), std::bad_variant_access);

    list.add(3);
    list.add(4);
    list.add(5);

    EXPECT_EQ(list.getAsList(), (std::vector<int>{2, 3, 4, 5}));
}

TEST_F(OneOrListTest, ShouldGetAsSingle)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_EQ(list.getAsSingle(), 2);

    list.add(3);
    list.add(4);
    list.add(5);

    EXPECT_THROW(list.getAsSingle(), std::bad_variant_access);
}

TEST_F(OneOrListTest, ShouldGetTryGetAsList)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_FALSE(list.tryGetAsList());

    list.add(3);
    list.add(4);
    list.add(5);

    EXPECT_EQ(*list.tryGetAsList(), (std::vector<int>{2, 3, 4, 5}));
}

TEST_F(OneOrListTest, ShouldTryGetAsSingle)
{
    sb::di::OneOrList<int> list{2};

    EXPECT_EQ(*list.tryGetAsSingle(), 2);

    list.add(3);
    list.add(4);
    list.add(5);

    EXPECT_FALSE(list.tryGetAsSingle());
}
