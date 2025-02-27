#include <iostream>
#include <span>
#include "llParam/LlParam.hpp"

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
        static_assert(llParam::obj_count_v<Sdk, Ts...> == 1);
        static_assert(llParam::is_obj1_v<Scl, Ts...>);

        config.sdkPin = llParam::get_obj<Sdk>(ts...).value;
        // better (for big objects)
        config.sclPin = llParam::get_obj<Scl>(std::forward<Ts>(ts)...).value;
        // llParam::args == std::forward
        // FUNC_v<>() == FUNC<>().value
        config.address = llParam::get_obj_v<Address>(llParam::args<Ts>(ts)...);
        using namespace llParam;
        config.baudrate = get_obj_or<Baudrate>(Baudrate{100'000}, args<Ts>(ts)...).value;
        // default constructor
        config.flags = get_obj_opt<Flags>(args<Ts>(ts)...).value;
    }

    template <typename ...Ts>
    bool write(std::span<uint8_t> data, Ts&&... ts)
    {
        // int timeout = llParam::get_obj_or_v(Timeout{100}, ts...);
        // i2c_write...(data, timeout);
        for(auto it=data.begin(); it < data.end(); ++it){
            printf("0x%x,", *it);
        }
        printf("\n");
        printf("timeout: %d\n", llParam::get_obj_or_v(Timeout{100}, ts...));
        // printf("timeout: %d\n", timeout);
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
