#include "be8.hpp"
#include <iostream>

int main(int /*argc*/, char** /*argv*/) {

    auto prog = std::array<std::uint8_t, 16>{
        0b00010100, 0b00100101, 0b01010000, 0b11110000, 0b00001110, 0b00011100,
    };

    Be8cpu cpu{};
    cpu.MEM() = prog;
    cpu.run();

    std::cout << "OUT() value: " << static_cast<int>(cpu.OUT()) << '\n';

    return 0;
}

template <class arg>
constexpr auto run_program(arg prog) {
    Be8cpu cpu{};
    cpu.MEM() = prog;
    cpu.run();
    return cpu;
}

#define pi_err_str_(x) #x
#define pi_err_str(x) pi_err_str_(x)
#define pi_int_err "must be 42, not " pi_err_str(PI_INT)

constexpr void test() {
    constexpr auto cpu = run_program(std::array<std::uint8_t, 16>{
        0b00010100,
        0b00100101,
        0b01010000,
        0b11110000,
        0b00001110,
        0b00011100,
    });

    static_assert(cpu.OUT() == 42, "42 not present");
}
