#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <regex>
#include <string>
#include <variant>
#include <vector>

struct Instruction {
    enum class ParamType {
        Integer,
        Label,
        None,
    };

    uint8_t opcode;
    ParamType param_type;

    std::variant<uint8_t, std::string> data;
};
static const std::map<std::string, Instruction> instructions{
    { "NOP", { 0b0000, Instruction::ParamType::None } },    { "LDA", { 0b0001, Instruction::ParamType::Integer } }, { "ADD", { 0b0010, Instruction::ParamType::Integer } },
    { "SUB", { 0b0011, Instruction::ParamType::Integer } }, { "STA", { 0b0100, Instruction::ParamType::Integer } }, { "LDI", { 0b0101, Instruction::ParamType::Integer } },
    { "JMP", { 0b0110, Instruction::ParamType::Label } },   { "JC", { 0b1011, Instruction::ParamType::Label } },    { "OUT", { 0b1110, Instruction::ParamType::None } },
    { "HLT", { 0b1111, Instruction::ParamType::None } },
};

class Parser {
public:
    bool parse_line(const std::vector<std::string>& tokens);

    void pretty_print_parsed_instructions();

private:
    std::map<std::string, int> labels;
    std::vector<Instruction> parsed_instructions;
};

std::vector<std::string> tokenize_line(const std::string& line);

int main(int argc, char** argv) {
    std::map<int, std::vector<std::string>> token_lines;
    auto line_number = 1;

    for (std::string line; std::getline(std::cin, line);) {
        auto tokens = tokenize_line(line);
        if (!tokens.empty()) {
            token_lines[line_number] = tokens;
        }
        line_number++;
    }
    {
        Parser parser;
        for (const auto& [line, tokens] : token_lines) {
            const auto result = parser.parse_line(tokens);
            if (!result) {
                std::cout << "Parse error on line " << line << '\n';
                return -1;
            }
        }
        parser.pretty_print_parsed_instructions();
    }

    return 0;
}

std::vector<std::string> tokenize_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream line_stream;
    {
        // Remove comments
        const auto r = std::regex(";.*$");
        std::regex_replace(std::ostream_iterator<char>(line_stream), line.begin(), line.end(), r, "");
    }

    std::string buf;
    for (auto c : line_stream.str()) {
        if (std::isspace(c)) {
            if (!buf.empty()) {
                tokens.push_back(buf);
                buf = "";
            }
            continue;
        }
        buf += c;
    }

    if (!buf.empty()) {
        tokens.push_back(buf);
    }
    return tokens;
};

static bool is_valid_param_label(const std::string& label) {
    if (label.size() <= 0) {
        return false;
    }
    return isalpha(label[0]);
}

bool Parser::parse_line(const std::vector<std::string>& tokens) {
    if (tokens.size() <= 0) {
        return false;
    }

    const std::regex decimal_regex("\\d*");

    for (int i = 0; i < tokens.size(); i++) {
        const auto& token = tokens[i];
        {
            // Instruction
            auto uppercase = token;
            std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), [](unsigned char c) { return std::toupper(c); });
            auto iter = instructions.find(uppercase);

            if (iter != instructions.end()) {
                // std::cout << "found instruction: " << uppercase << '\n';
                auto instruction = iter->second;
                if (instruction.param_type == Instruction::ParamType::None) {
                    parsed_instructions.push_back(instruction);
                    continue;
                }
                if ((i + 1) >= tokens.size()) {
                    return false;
                }
                i++;
                const auto data = tokens[i];
                switch (instruction.param_type) {
                case Instruction::ParamType::Integer: {
                    std::string::size_type sz;
                    uint8_t val = 0;
                    try {
                        val = std::stoi(data, &sz);
                    } catch (...) {
                        return false;
                    }
                    if (sz != data.size()) {
                        return false;
                    }
                    instruction.data = val;
                    break;
                }
                case Instruction::ParamType::Label: {
                    if (!is_valid_param_label(data)) {
                        return false;
                    }
                    instruction.data = data;
                    break;
                }
                }
                parsed_instructions.push_back(instruction);
                continue;
            }
        }
        {
            // Label
            if (token.size() > 1 && token.back() == ':') {
                int ip = parsed_instructions.size();
                const auto name = token.substr(0, token.size() - 1);
                labels[name] = ip;
                continue;
            }
        }
        return false; // Could not parse
    }
    return true;
}

void Parser::pretty_print_parsed_instructions() {
    std::cout << "Labels:\n";

    for (const auto& [name, addr] : labels) {
        std::cout << '\t' << addr << ":\t" << name << '\n';
    }

    std::cout << "--------------\nInstructions:\n";

    for (const auto& instruction : parsed_instructions) {
        std::cout << "\topcode: " << std::bitset<4>(instruction.opcode) << '\t';
        if (std::holds_alternative<std::string>(instruction.data)) {
            std::cout << std::get<std::string>(instruction.data);
        }
        if (instruction.param_type == Instruction::ParamType::Integer) {
            std::cout << std::bitset<4>(std::get<uint8_t>(instruction.data));
        }
        std::cout << '\n';
    }
}
