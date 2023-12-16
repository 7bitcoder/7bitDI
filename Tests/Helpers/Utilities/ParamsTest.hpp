
#include "SevenBit/Conf/Json.hpp"
#include <gtest/gtest.h>
#include <string>
#include <tuple>

template <class... T> using Param = std::tuple<T...>;
template <class... T> using Params = std::vector<Param<T...>>;
template <class T> using OneParams = std::vector<T>;

template <class T> struct TypeExtractor
{
    using value_type = typename T::iterator::value_type;
};

template <class... T> struct TypeExtractor<testing::internal::CartesianProductHolder<T...>>
{
    using value_type = typename std::tuple<typename TypeExtractor<T>::value_type...>;
};
#define SETUP_PARAMS_TEST(group, name, values)                                                                         \
    struct name : public testing::TestWithParam<TypeExtractor<decltype(values)>::value_type>                           \
    {                                                                                                                  \
    };                                                                                                                 \
    INSTANTIATE_TEST_SUITE_P(group, name, values);

#define PARAMS_TEST(group, name, values)                                                                               \
    SETUP_PARAMS_TEST(group, name, testing::ValuesIn(values))                                                          \
    TEST_P(name, )

#define PARAMS_TEST_COMBINED_1(group, name, one)                                                                       \
    SETUP_PARAMS_TEST(group, name, testing::Combine(testing::ValuesIn(one))) TEST_P(name, )

#define PARAMS_TEST_COMBINED_2(group, name, one, two)                                                                  \
    SETUP_PARAMS_TEST(group, name, testing::Combine(testing::ValuesIn(one), testing::ValuesIn(two)))                   \
    TEST_P(name, )

#define PARAMS_TEST_COMBINED_3(group, name, one, two, three)                                                           \
    SETUP_PARAMS_TEST(group, name,                                                                                     \
                      testing::Combine(testing::ValuesIn(one), testing::ValuesIn(two), testing::ValuesIn(three)))      \
    TEST_P(name, )

#define PARAMS_TEST_COMBINED_4(group, name, one, two, three, four)                                                     \
    SETUP_PARAMS_TEST(group, name,                                                                                     \
                      testing::Combine(testing::ValuesIn(one), testing::ValuesIn(two), testing::ValuesIn(three),       \
                                       testing::ValuesIn(four)))                                                       \
    TEST_P(name, )

#define PARAMS_TEST_COMBINED_5(group, name, one, two, three, four, five)                                               \
    SETUP_PARAMS_TEST(group, name,                                                                                     \
                      testing::Combine(testing::ValuesIn(one), testing::ValuesIn(two), testing::ValuesIn(three),       \
                                       testing::ValuesIn(four), testing::ValuesIn(five)))                              \
    TEST_P(name, )
