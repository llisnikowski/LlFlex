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


template <typename T>
constexpr std::enable_if_t<std::is_default_constructible_v<T>, T> 
get_obj_opt()
{
    return T{}; //default constructor
}
template <typename T>
constexpr std::enable_if_t<!std::is_default_constructible_v<T>, T> 
get_obj_opt()
{
    static_assert(std::is_default_constructible_v<T>, "No default constructor");
}

template <typename T, typename T1, typename ...Ts>
constexpr T get_obj_opt(T1 t1, Ts... ts)
{
    if constexpr (std::is_same_v<T, T1>) {
        return t1;
    }
    else {
        return get_obj_opt<T>(ts...);
    }
}

template <typename T, typename ...Ts>
constexpr T get_obj(Ts... ts)
{
    static_assert(is_obj_v<T, Ts...>, "No object type");
    if constexpr(is_obj_v<T, Ts...>){
        return get_obj_opt<T>(ts...);
    }
}


template <typename T>
constexpr T get_obj_or(T def)
{
    return def;
}

template <typename T, typename T1, typename ...Ts>
constexpr T get_obj_or(T def, T1 t1, Ts... ts)
{
    if constexpr (std::is_same_v<T, T1>) {
        return t1;
    }
    else {
        return get_obj_or<T>(def, ts...);
    }
}


template <typename T, std::size_t N>
constexpr std::enable_if_t<std::is_default_constructible_v<T>, T> 
get_objN_opt()
{
    return T{}; //default constructor
}
template <typename T, std::size_t N>
constexpr std::enable_if_t<!std::is_default_constructible_v<T>, T> 
get_objN_opt()
{
    static_assert(std::is_default_constructible_v<T>, "No default constructor");
}

template <typename T, std::size_t N, typename T1, typename ...Ts>
constexpr T get_objN_opt(T1 t1, Ts... ts)
{
    if constexpr (std::is_same_v<T, T1>) {
        if constexpr (N == 0){
            return t1;
        }
        else {
            return get_objN_opt<T, N-1>(ts...);
        }
    }
    else {
        return get_objN_opt<T, N>(ts...);
    }
}

template <typename T, std::size_t N, typename ...Ts>
constexpr T get_objN(Ts... ts)
{
    static_assert(obj_count_v<T, Ts...> >= N+1, "Wrong amount of object type");
    if constexpr(obj_count_v<T, Ts...> >= N+1){
        return get_objN_opt<T, N>(ts...);
    }
}

template <typename T, std::size_t N>
constexpr T get_objN_or(T def)
{
    return def;
}

template <typename T, std::size_t N, typename T1, typename ...Ts>
constexpr T get_objN_or(T def, T1 t1, Ts... ts)
{
    if constexpr (std::is_same_v<T, T1>) {
        if constexpr (N == 0){
            return t1;
        }
        else {
            return get_objN_or<T, N-1>(def, ts...);
        }
    }
    else {
        return get_objN_or<T, N>(def, ts...);
    }
}


} // namespace llFlex
