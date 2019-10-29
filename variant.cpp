#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>
#include <variant>
#include <iomanip>

void exec();

template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...) -> overload<Ts...>;

int main() {
    exec();
    return 0;
}

struct parsing_init {
};

struct parsing_char {
    std::string current_result;
};

struct parsing_space {
    int number_of_spaces;
};

struct parsing_special {
    char first_special_character;
};

using parser_state = std::variant<parsing_init, parsing_char, parsing_space, parsing_special>;

bool is_char(char c) {
    return c >= 'A' && c <= 'z';
}

bool is_special(char c) {
    return c == '!' || c == '?';
}

void parse(const std::string& s) {
    auto state = parser_state{parsing_init{}};
    for (auto c : s) {
        state = std::visit(overload{
                [=](parsing_init &state) -> parser_state {
                    if (c == ' ')
                        return parsing_space{1};
                    else if (is_char(c))
                        return parsing_char{std::string{c}};
                    else if (is_special(c))
                        return parsing_special{c};
                    throw std::exception();
                },
                [=](parsing_char &state) -> parser_state {
                    if (is_char(c)) {
                        state.current_result += c;
                        return state;
                    } else {
                        std::cout << "Scanned word: " << state.current_result << std::endl;
                        if (c == ' ') {
                            return parsing_space{1};
                        } else if (is_special(c)) {
                            return parsing_special{c};
                        }
                    }
                    throw std::exception();
                },
                [=](parsing_space &state) -> parser_state {
                    if (c == ' ') {
                        state.number_of_spaces++;
                        return state;
                    } else {
                        std::cout << "Scanned " << state.number_of_spaces << " spaces" << std::endl;
                        if (is_char(c))
                            return parsing_char{std::string{c}};
                        else
                            return parsing_special{c};
                    }
                },
                [=](parsing_special &state) -> parser_state {
                    if (is_special(c))
                        return state;
                    else
                    {
                        std::cout << "The first special character was " << state.first_special_character << std::endl;
                        if (c == ' ')
                            return parsing_space{1};
                        else if (is_char(c))
                            return parsing_char{std::string{c}};
                    }
                    throw std::exception();
                }
        }, state);
    }
    std::visit(overload{
        [=](parsing_space& state) {
            std::cout << "Ended with parsing " << state.number_of_spaces << std::endl;
        },
        [=](parsing_special& state) {
            std::cout << "Ended with parsing special chars, first of which was " << state.first_special_character;
        },
        [=](parsing_init& state) {
            std::cout << "Seems like the string was empty" << std::endl;
        },
        [=](parsing_char& state) {
            std::cout << "Input ends with '" << state.current_result << "'" << std::endl;
        }
        // ...
    }, state);
}

void exec() {
    parse("thomas    kowalski!");
}