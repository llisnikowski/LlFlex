#include <type_traits>

namespace llFlex
{

template <typename ...Ts>
struct is_obj;
template <typename T, typename T1, typename ...Ts>
struct is_obj<T, T1, Ts...>
{
    using value_type = std::conditional_t<
        std::is_same_v<T, T1>, 
        std::true_type,
        typename is_obj<T, Ts...>::value_type
    >;
};
template <typename T>
struct is_obj<T>{
    using value_type = std::false_type;
};

template <typename ...Ts>
static constexpr bool is_obj_v = is_obj<Ts...>::value_type::value;


template <typename ...Ts>
struct obj_count;
template <typename T, typename T1, typename ...Ts>
struct obj_count<T, T1, Ts...>
{
    static constexpr std::size_t count = 
        (std::is_same_v<T, T1> ? 1 : 0)
        + obj_count<T, Ts...>::count;
};
template <typename T>
struct obj_count<T>{
    static constexpr std::size_t count = 0;
};

template <typename ...Ts>
static constexpr std::size_t obj_count_v = obj_count<Ts...>::count;


template <typename ...Ts>
static constexpr bool is_obj1_v = obj_count_v<Ts...> == 1;
template <typename ...Ts>
static constexpr bool is_obj0_v = obj_count_v<Ts...> == 0; // equivalent !is_obj_v



} // namespace llFlex
