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

template <typename T>
class ParamT{};
template <typename T>
class ParamT2{};

template <typename T>
using Pt = ParamT<T>;
template <typename T>
using Pt2 = ParamT2<T>;

struct ParamNdc
{
    // ParamNdc() = delete; // No default constructor
    ParamNdc(std::string value)
    :value{value}
    {}
    std::string value;
    bool operator==(const ParamNdc &rhs) const{
        return this->value == rhs.value;
    }
    bool operator==(const ParamNdc &&rhs) const{
        return this->value == rhs.value;
    }
};
using Pn = ParamNdc;


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

TEST(Flex_test, getObjectOptional)
{
    EXPECT_EQ((llFlex::get_obj_opt<P1>(P2{"p2"}, P3{"p3"}, P1{"p1"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_opt<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{}); //default constructor
    EXPECT_EQ((llFlex::get_obj_opt<P1>(P1{"p1"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_opt<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"}, P1{"p1"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_opt<P1>(P2{"p2"}, P3{"p3"}, P1{"p1_1"}, P2{"p2_2"}, P1{"p1_2"}, P1{"p1_3"})), P1{"p1_1"});
    EXPECT_EQ((llFlex::get_obj_opt<Pn>(P2{"p2"}, P3{"p3"}, Pn{"Pn"}, P2{"p2_2"})), Pn{"Pn"});
    // assert: no default constructor
    // EXPECT_EQ((llFlex::get_obj_opt<Pn>(P2{"p2"}, P3{"p3"}, P2{"p2_2"})), Pn{"Pn"});
}


TEST(Flex_test, getObject)
{
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P1{"p1"}, P2{"p2_2"})), P1{"p1"});
    // assert: no object
    // EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{}); //default constructor
    EXPECT_EQ((llFlex::get_obj<P1>(P1{"p1"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P2{"p2_2"}, P1{"p1"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj<P1>(P2{"p2"}, P3{"p3"}, P1{"p1_1"}, P2{"p2_2"}, P1{"p1_2"}, P1{"p1_3"})), P1{"p1_1"});
    EXPECT_EQ((llFlex::get_obj<Pn>(P2{"p2"}, P3{"p3"}, Pn{"Pn"}, P2{"p2_2"})), Pn{"Pn"});
    // assert: no object
    // EXPECT_EQ((llFlex::get_obj<Pn>(P2{"p2"}, P3{"p3"}, P2{"p2_2"})), Pn{"Pn"});
}

TEST(Flex_test, getObjectOr)
{
    EXPECT_EQ((llFlex::get_obj_or<P1>(P1{"default"}, P2{"p2"}, P3{"p3"}, P1{"p1"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_or<P1>(P1{"default"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{"default"}); //default constructor
    EXPECT_EQ((llFlex::get_obj_or<P1>(P1{"default"}, P1{"p1"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_or<P1>(P1{"default"}, P2{"p2"}, P3{"p3"}, P2{"p2_2"}, P1{"p1"})), P1{"p1"});
    EXPECT_EQ((llFlex::get_obj_or<P1>(P1{"default"}, P2{"p2"}, P3{"p3"}, P1{"p1_1"}, P2{"p2_2"}, P1{"p1_2"}, P1{"p1_3"})), P1{"p1_1"});
}

TEST(Flex_test, getObjectNumberOptional)
{
    EXPECT_EQ((llFlex::get_objN_opt<P1, 0>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_1"});
    EXPECT_EQ((llFlex::get_objN_opt<P1, 1>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_2"});
    EXPECT_EQ((llFlex::get_objN_opt<P1, 2>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_3"});
    EXPECT_EQ((llFlex::get_objN_opt<P1, 2>(P1{"p1_1"}, P3{"p3"}, P2{"p2_2"})), P1{}); //default constructor
    EXPECT_EQ((llFlex::get_objN_opt<Pn, 1>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_2"});
    EXPECT_EQ((llFlex::get_objN_opt<Pn, 2>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_3"});
    // EXPECT_EQ((llFlex::get_objN_opt<Pn, 2>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"})), Pn{"pn_3"});  //No default constructor
}

TEST(Flex_test, getObjectNumber)
{
    EXPECT_EQ((llFlex::get_objN<P1, 0>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_1"});
    EXPECT_EQ((llFlex::get_objN<P1, 1>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_2"});
    EXPECT_EQ((llFlex::get_objN<P1, 2>(P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_3"});
    // EXPECT_EQ((llFlex::get_objN<P1, 1>(P1{"p1_1"}, P3{"p3"}, P2{"p2_2"})), P1{}); //Only 1 object
    EXPECT_EQ((llFlex::get_objN<Pn, 1>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_2"});
    EXPECT_EQ((llFlex::get_objN<Pn, 2>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_3"});
    // llFlex::get_objN<Pn, 2>(Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"});  //Only 1 object
}

TEST(Flex_test, getObjectNumberOr)
{
    EXPECT_EQ((llFlex::get_objN_or<P1, 0>(P1{"def"}, P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_1"});
    EXPECT_EQ((llFlex::get_objN_or<P1, 1>(P1{"def"}, P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_2"});
    EXPECT_EQ((llFlex::get_objN_or<P1, 2>(P1{"def"}, P1{"p1_1"}, P3{"p3"}, P1{"p1_2"}, P2{"p2_2"}, P1{"p1_3"})), P1{"p1_3"});
    EXPECT_EQ((llFlex::get_objN_or<P1, 1>(P1{"def"}, P1{"p1_1"}, P3{"p3"}, P2{"p2_2"})), P1{"def"}); //Only 1 object
    EXPECT_EQ((llFlex::get_objN_or<Pn, 1>(Pn{"def"}, Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_2"});
    EXPECT_EQ((llFlex::get_objN_or<Pn, 2>(Pn{"def"}, Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"}, Pn{"pn_3"})), Pn{"pn_3"});
    EXPECT_EQ((llFlex::get_objN_or<Pn, 2>(Pn{"def"}, Pn{"pn_1"}, P3{"p3"}, Pn{"pn_2"}, P2{"p2_2"})), Pn{"def"});
}


TEST(Flex_test, getInnerType)
{
    EXPECT_TRUE((std::is_same_v<llFlex::get_inner_t<Pt<int>>, int>));
    EXPECT_FALSE((std::is_same_v<llFlex::get_inner_t<Pt<std::string>>, int>));
    EXPECT_TRUE((std::is_same_v<llFlex::get_inner_t<Pt<Pt<int>>>, Pt<int>>));
    EXPECT_TRUE((std::is_same_v<llFlex::get_inner_t<Pt<llFlex::get_inner_t<Pt<int>>>>, int>));
    EXPECT_TRUE((std::is_same_v<llFlex::get_inner_t<int>, nullptr_t>));
}

TEST(Flex_test, isTemplateType)
{
    EXPECT_TRUE((llFlex::is_template_v<Pt<int>>));
    EXPECT_FALSE((llFlex::is_template_v<P1>));
}
