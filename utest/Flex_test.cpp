#include "gtest/gtest.h"
#include "llFlex/LlFlex.hpp"


template <std::size_t N>
struct Param
{
    std::string value;
    constexpr bool operator==(const Param<N> &rhs) const{
        return this->value == rhs.value;
    }
    constexpr bool operator==(const Param<N> &&rhs) const{
        return this->value == rhs.value;
    }
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

TEST(Flex_test, isOneObject)
{
    EXPECT_TRUE((llFlex::is_obj1_v<P1, P2, P3, P1, P2>));
    EXPECT_FALSE((llFlex::is_obj1_v<P1, P2, P3, P2>));
    EXPECT_TRUE((llFlex::is_obj1_v<P1, P1, P3, P2>));
    EXPECT_TRUE((llFlex::is_obj1_v<P1, P2, P3, P2, P1>));
    EXPECT_FALSE((llFlex::is_obj1_v<P1, P2, P3, P1, P2, P1, P1>));
}

TEST(Flex_test, isZeroObject)
{
    EXPECT_FALSE((llFlex::is_obj0_v<P1, P2, P3, P1, P2>));
    EXPECT_TRUE((llFlex::is_obj0_v<P1, P2, P3, P2>));
    EXPECT_FALSE((llFlex::is_obj0_v<P1, P1, P3, P2>));
    EXPECT_FALSE((llFlex::is_obj0_v<P1, P2, P3, P2, P1>));
    EXPECT_FALSE((llFlex::is_obj0_v<P1, P2, P3, P1, P2, P1, P1>));
}

TEST(Flex_test, getObject)
{
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P1{"p1"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{}); //default constructor
    EXPECT_EQ((llFlex::get_obj<P1>(P1{"p1"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"}, P1{"p1"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P1{"p1_1"}, P2{"p2_2"}, P1{"p1_2"}, P1{"p1_3"})), P1{"p1_1"});
}


