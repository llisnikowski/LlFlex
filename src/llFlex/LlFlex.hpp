#include <type_traits>
#include <utility>

namespace llFlex
{

template <typename T1, typename T2>
constexpr bool is_same_obj = std::is_same_v<
    std::remove_reference_t<T1>,
    std::remove_reference_t<T2>
>;


// args == std::forward
template<typename T>
[[__nodiscard__]]
inline constexpr T&& args(std::remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t); 
}

template<typename T>
[[__nodiscard__]]
inline constexpr T&& args(std::remove_reference_t<T>&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value,
    "std::forward must not be used to convert an rvalue to an lvalue");
    return static_cast<T&&>(t);
}


template <typename ...Ts>
struct is_obj;
template <typename T, typename T1, typename ...Ts>
struct is_obj<T, T1, Ts...>
{
    using value_type = std::conditional_t<
        is_same_obj<T, T1>,
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
        (is_same_obj<T, T1> ? 1 : 0)
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
inline constexpr std::enable_if_t<std::is_default_constructible_v<T>, T>
get_obj_opt()
{
    return T{}; //default constructor
}
template <typename T>
inline constexpr std::enable_if_t<!std::is_default_constructible_v<T>, T> 
get_obj_opt()
{
    static_assert(std::is_default_constructible_v<T>, "No default constructor");
}

template <typename T, typename T1, typename ...Ts>
inline constexpr T get_obj_opt(T1&& t1, Ts&&... ts)
{
    if constexpr (is_same_obj<T, T1>) {
        return std::forward<T>(t1);
    }
    else {
        return get_obj_opt<T>(std::forward<Ts>(ts)...);
    }
}

template <typename T, typename ...Ts>
inline constexpr T get_obj(Ts&&... ts)
{
    static_assert(is_obj_v<T, Ts...>, "No object type");
    if constexpr(is_obj_v<T, Ts...>){
        return get_obj_opt<T>(std::forward<Ts>(ts)...);
    }
}

template <typename T>
inline constexpr T&& get_obj_or(T&& def)
{
    return std::forward<T>(def);
}

template <typename T, typename T1, typename ...Ts>
inline constexpr T&& get_obj_or(T&& def, T1&& t1, Ts&&... ts)
{
    if constexpr (is_same_obj<T, T1>) {
        return std::forward<T>(t1);
    }
    else {
        return get_obj_or<T>(std::forward<T>(def), std::forward<Ts>(ts)...);
    }
}


template <typename T, std::size_t N>
inline constexpr std::enable_if_t<std::is_default_constructible_v<T>, T> 
get_objN_opt()
{
    return T{}; //default constructor
}
template <typename T, std::size_t N>
inline constexpr std::enable_if_t<!std::is_default_constructible_v<T>, T> 
get_objN_opt()
{
    static_assert(std::is_default_constructible_v<T>, "No default constructor");
}

template <typename T, std::size_t N, typename T1, typename ...Ts>
inline constexpr T get_objN_opt(T1&& t1, Ts&&... ts)
{
    if constexpr (is_same_obj<T, T1>) {
        if constexpr (N == 0){
            return std::forward<T>(t1);
        }
        else {
            return get_objN_opt<T, N-1>(std::forward<Ts>(ts)...);
        }
    }
    else {
        return get_objN_opt<T, N>(std::forward<Ts>(ts)...);
    }
}

template <typename T, std::size_t N, typename ...Ts>
inline constexpr T get_objN(Ts&&... ts)
{
    static_assert(obj_count_v<T, Ts...> >= N+1, "Wrong amount of object type");
    if constexpr(obj_count_v<T, Ts...> >= N+1){
        return get_objN_opt<T, N>(std::forward<Ts>(ts)...);
    }
}

template <typename T, std::size_t N>
inline constexpr T&& get_objN_or(T&& def)
{
    return std::forward<T>(def);
}

template <typename T, std::size_t N, typename T1, typename ...Ts>
inline constexpr T&& get_objN_or(T&& def, T1&& t1, Ts&&... ts)
{
    if constexpr (is_same_obj<T, T1>) {
        if constexpr (N == 0){
            return std::forward<T>(t1);
        }
        else {
            return get_objN_or<T, N-1>(std::forward<T>(def), std::forward<Ts>(ts)...);
        }
    }
    else {
        return get_objN_or<T, N>(std::forward<T>(def), std::forward<Ts>(ts)...);
    }
}


template <typename T, typename ...Ts>
inline constexpr decltype(T::value) get_obj_opt_v(Ts&&... ts) {return get_obj_opt<T>(args<Ts>(ts)...).value;}
template <typename T, typename ...Ts>
inline constexpr decltype(T::value) get_obj_v(Ts&&... ts) {return get_obj<T>(args<Ts>(ts)...).value;}
template <typename T, typename ...Ts>
inline constexpr decltype(T::value) get_obj_or_v(T&& t, Ts&&... ts) {return get_obj_or(args<T>(t), args<Ts>(ts)...).value;}

template <typename T, std::size_t N, typename ...Ts>
inline constexpr decltype(T::value) get_objN_opt_v(Ts&&... ts) {return get_objN_opt<T, N>(args<Ts>(ts)...).value;}
template <typename T, std::size_t N, typename ...Ts>
inline constexpr decltype(T::value) get_objN_v(Ts&&... ts) {return get_objN<T, N>(args<Ts>(ts)...).value;}
template <typename T, std::size_t N, typename ...Ts>
inline constexpr decltype(T::value) get_objN_or_v(T&& t, Ts&&... ts) {return get_objN_or<T, N>(args<T>(t), args<Ts>(ts)...).value;}


template <typename ...T>
struct get_inner;
template <template<class> typename T, typename S>
struct get_inner<T<S>>
{
    using type = S;
};
template <typename T>
struct get_inner<T>
{
    using type = nullptr_t;
};
template <typename T>
using get_inner_t = typename get_inner<T>::type;


template <typename T>
struct is_template : public std::false_type
{};
template <template<class> typename Tt, typename T>
struct is_template<Tt<T>> : public std::true_type
{};
template <typename T>
constexpr bool is_template_v = is_template<T>::value;


template <template<class> typename Tt, typename ...Ts>
struct get_from
{};
template <template<class> typename Tt ,typename Ts1, typename ...Ts>
struct get_from <Tt, Tt<Ts1>, Ts...>
{
    using type = Ts1;
};
template <template<class> typename Tt ,typename Ts1, typename ...Ts>
struct get_from <Tt, Ts1, Ts...>
{
    using type = typename get_from<Tt, Ts...>::type;
};
template <template<class> typename Tt ,typename ...Ts>
using get_from_t = typename get_from<Tt, Ts...>::type;


template <template<class> typename Tt, typename T, typename ...Ts>
struct get_from_or
{
    using type = T;
};
template <template<class> typename Tt, typename T ,typename Ts1, typename ...Ts>
struct get_from_or <Tt, T, Tt<Ts1>, Ts...>
{
    using type = Ts1;
};
template <template<class> typename Tt, typename T ,typename Ts1, typename ...Ts>
struct get_from_or <Tt, T, Ts1, Ts...>
{
    using type = typename get_from_or<Tt, T, Ts...>::type;
};
template <template<class> typename Tt, typename T ,typename ...Ts>
using get_from_or_t = typename get_from_or<Tt, T, Ts...>::type;


} // namespace llFlex
