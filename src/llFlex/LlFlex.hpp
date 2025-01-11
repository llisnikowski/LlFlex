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


} // namespace llFlex
