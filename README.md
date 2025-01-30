# LlFlex
**LlFlex** is a template-based library for C++ that simplifies and enhances the creation of constructors and functions. It provides an intuitive way to pass any number of parameters, in any order, depending on the current requirements, without the need to write multiple overloaded functions.

## Features
- **Flexible Parameter Handling**: Pass any number of parameters to functions, in any order, depending on your needs, without writing multiple overloaded functions.
- **Clean Code Structure**: Reduce code duplication and provide simple, easy-to-understand functions.
- **Extensibility**: Easily adapt and extend constructors and functions.
- **Modern C++ Compatibility**: Designed with modern C++ standards in mind.
- **Performance**: Optimized for minimal performance and memory impact, utilizing `constexpr` and `inline` for compile-time evaluation and reduced runtime overhead.

## Examples
```cpp
class I2c
{
public:
    struct Sdk{uint8_t value{};};
    struct Scl{uint8_t value{};};
    struct Address{uint8_t value{};};
    struct Baudrate{int value{};};
    struct Flags{uint8_t value{};};
    struct Timeout{int value{};};

    template <typename ...Ts>
    I2c(Ts&&... ts)
    {
        static_assert(llFlex::obj_count_v<Sdk, Ts...> == 1);
        static_assert(llFlex::is_obj1_v<Scl, Ts...>);

        config.sdkPin = llFlex::get_obj<Sdk>(ts...).value;
        // better for big objects
        config.sclPin = llFlex::get_obj<Scl>(std::forward<Ts>(ts)...).value;
        // llFlex::args == std::forward
        // FUNC_v<>() == FUNC<>().value
        config.address = llFlex::get_obj_v<Address>(llFlex::args<Ts>(ts)...);
        using namespace llFlex;
        config.baudrate = get_obj_or_v<Baudrate>(Baudrate{100'000}, args<Ts>(ts)...);
        // default constructor if not exist
        config.flags = get_obj_opt_v<Flags>(args<Ts>(ts)...);
    }

    template <typename ...Ts>
    bool write(std::span<uint8_t> data, Ts&&... ts)
    {
        int timeout = llFlex::get_obj_or_v(Timeout{100}, ts...);
        // i2c_write...(data, timeout);
        return true;
    }

private:
    struct Config {
        uint8_t sdkPin{};
        uint8_t sclPin{};
        uint8_t address{};
        uint8_t flags{};
        int baudrate{};
    } config;
};


int main()
{
    I2c i2c{I2c::Sdk{10}, I2c::Scl{11}, I2c::Address{0x12}};
    uint8_t data[]{1, 2, 3};
    i2c.write(data);
    i2c.write(data, I2c::Timeout{100});
}
```

## Functions and Traits
The following functions are simplified for demonstration purposes. They do not include advanced features such as `constexpr`, `inline`, or `universal references (T&&)`.

Variables:
- **T** - main type.
- **Ts...** - variadic arguments types.
- **ts** - variadic objects.
- **N** - object number.

### Functions
```cpp
template <typename T, typename ...Ts>
constexpr T get_obj_opt(Ts... ts);
template <typename T, typename ...Ts>
constexpr T get_obj(Ts... ts);
template <typename T, typename ...Ts>
constexpr T get_obj_or(T default_value, Ts... ts);

template <typename T, std::size_t N, typename ...Ts>
constexpr T get_objN_opt(Ts... ts);
template <typename T, std::size_t N, typename ...Ts>
constexpr T get_objN(Ts... ts);
template <typename T, std::size_t N, typename ...Ts>
constexpr T get_objN_or(T default_value, Ts... ts);
```

FUNC_v() == FUNC().value
```
get_obj_opt_v(...)   ->  get_obj_opt(...).value
get_obj_v(...)       ->  get_obj(...).value
get_obj_or_v(...)    ->  get_obj_or(...).value
get_objN_opt_v(...)  ->  get_objN_opt(...).value
get_objN_v(...)      ->  get_objN(...).value
get_objN_or_v(...)   ->  get_objN_or(...).value
```

### Helpful
```cpp
template <typename T1, typename T2>
constexpr bool is_same_obj = std::is_same_v<
    std::remove_reference_t<T1>,
    std::remove_reference_t<T2>
>;

// args == std::forward
template<typename T>
constexpr T&& args(std::remove_reference_t<T>& t) noexcept
template<typename T>
constexpr T&& args(std::remove_reference_t<T>&& t) noexcept

```


### Traits
```cpp
template <typename T, typename ...Ts>
bool is_obj_v;

template <typename T, typename ...Ts>
std::size_t obj_count_v;
template <typename T, typename ...Ts>
std::size_t is_obj1_v; // obj_count_v<Ts...> == 1
template <typename T, typename ...Ts>
std::size_t is_obj0_v; // obj_count_v<Ts...> == 0
```

## Integration
### CMake
#### add_subdirectory
Clone repo:
`git clone git@github.com:llisnikowski/LlFlex.git`
or
`git submodule add git@github.com:llisnikowski/LlFlex.git`

and add it to CMake:
```cmake
add_subdirectory(path/to/LlFlex)
# ...
target_link_libraries(${PROJECT_NAME}
PRIVATE
    LlFlex::LlFlex
)
```
#### FetchContent
// todo
#### find_package
// todo

## License
This project is licensed under the MIT License.

## Contribution
Feel free to fork the repository, make changes, and submit a pull request. If you have any issues or suggestions, open an issue.

## Contact
Łukasz Liśnikowski l.lisnikowski@gmail.com <br>
Project Link: [https://github.com/llisnikowski/LlFlex](https://github.com/llisnikowski/LlFlex)

