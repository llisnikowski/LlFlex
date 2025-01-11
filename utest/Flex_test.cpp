#include "gtest/gtest.h"
#include "llFlex/LlFlex.hpp"


template <std::size_t N>
struct Param
{
    std::string value;
};
using P1 = Param<1>;
using P2 = Param<2>;
using P3 = Param<3>;
using P4 = Param<4>;
using P5 = Param<5>;

TEST(Flex_test, isObject)
{
    EXPECT_TRUE((llFlex::is_obj_v<P1, P2, P3, P1, P2>));
    EXPECT_FALSE((llFlex::is_obj_v<P1, P2, P3, P2>));
    EXPECT_TRUE((llFlex::is_obj_v<P1, P1, P3, P2>));
    EXPECT_TRUE((llFlex::is_obj_v<P1, P2, P3, P2, P1>));
    EXPECT_TRUE((llFlex::is_obj_v<P1, P2, P3, P1, P2, P1>));
}

TEST(Flex_test, objectCount)
{
    EXPECT_EQ((llFlex::obj_count_v<P1, P2, P3, P1, P2>), 1);
    EXPECT_EQ((llFlex::obj_count_v<P1, P2, P3, P2>), 0);
    EXPECT_EQ((llFlex::obj_count_v<P1, P1, P3, P2>), 1);
    EXPECT_EQ((llFlex::obj_count_v<P1, P2, P3, P2, P1>), 1);
    EXPECT_EQ((llFlex::obj_count_v<P1, P2, P3, P1, P2, P1, P1>), 3);
}
