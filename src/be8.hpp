#ifndef __BE_8_HPP__
#define __BE_8_HPP__

#include <array>
#include <stdint.h>

class Be8cpu {
private:
    std::uint8_t a_reg;
    std::uint8_t b_reg;
    std::uint8_t ir_reg;
    std::uint8_t pc;

    std::array<std::uint8_t, 16> memory;
    std::uint8_t out;

    bool halt;
    bool carry_flag;
    bool zero_flag;

    enum class OpCode : std::uint8_t {
        NOP = 0b0000,
        LDA = 0b0001,
        ADD = 0b0010,
        SUB = 0b0011,
        STA = 0b0100,
        OUT = 0b0101,
        JMP = 0b0110,
        LDI = 0b0111,
        JC = 0b1000,
        HLT = 0b1111,
    };

public:
    [[nodiscard]] constexpr auto& A() noexcept { return a_reg; }
    [[nodiscard]] constexpr const auto& A() const noexcept { return a_reg; }

    [[nodiscard]] constexpr auto& B() noexcept { return b_reg; }
    [[nodiscard]] constexpr const auto& B() const noexcept { return b_reg; }

    [[nodiscard]] constexpr auto& IR() noexcept { return ir_reg; }
    [[nodiscard]] constexpr const auto& IR() const noexcept { return ir_reg; }

    [[nodiscard]] constexpr auto& PC() noexcept { return pc; }
    [[nodiscard]] constexpr const auto& PC() const noexcept { return pc; }

    [[nodiscard]] constexpr auto& MEM() noexcept { return memory; }
    [[nodiscard]] constexpr const auto& MEM() const noexcept { return memory; }

    [[nodiscard]] constexpr auto& OUT() noexcept { return out; }
    [[nodiscard]] constexpr const auto& OUT() const noexcept { return out; }

    [[nodiscard]] constexpr auto& HALT() noexcept { return halt; }
    [[nodiscard]] constexpr const auto& HALT() const noexcept { return halt; }

    [[nodiscard]] constexpr auto& CF() noexcept { return carry_flag; }
    [[nodiscard]] constexpr const auto& CF() const noexcept { return carry_flag; }

    [[nodiscard]] constexpr auto& ZF() noexcept { return zero_flag; }
    [[nodiscard]] constexpr const auto& ZF() const noexcept { return zero_flag; }

    constexpr void run_step() noexcept {
        auto HI = [](const std::uint8_t val) constexpr noexcept { return (val & 0xf0) >> 4; };
        auto LO = [](const std::uint8_t val) constexpr noexcept { return (val & 0x0f); };
        IR() = MEM()[PC()];

        const auto op = static_cast<OpCode>(HI(IR()));
        const auto lo = LO(IR());

        switch (op) {
        case OpCode::NOP:
            break;
        case OpCode::LDA:
            A() = MEM()[lo];
            break;
        case OpCode::ADD:
            A() += MEM()[lo];
            break;
        case OpCode::SUB:
            //
            break;
        case OpCode::STA:
            //
            break;
        case OpCode::OUT:
            OUT() = A();
            break;
        case OpCode::JMP:
            //
            break;
        case OpCode::LDI:
            //
            break;
        case OpCode::JC:
            //
            break;
        case OpCode::HLT:
            HALT() = true;
            break;
        }
        PC()++;
    }

    constexpr void run() noexcept {
        while (!HALT()) {
            run_step();
        }
    };
};

#endif
