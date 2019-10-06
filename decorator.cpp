#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>
#include <optional>

void exec();

int main() {
    exec();
    return 0;
}

template<typename Function>
auto never_throw(Function &f) {
    return [&f](auto... args) -> decltype(f(args...)) {
        try {
            return f(args...);
        }
        catch (...) {
            std::cerr << "We got an exception" << std::endl;
            throw std::exception();
            // return decltype(f(args...))();
        }
    };
}

template<typename Function>
auto did_throw(Function &f) {
    return [&f](auto... args) -> bool {
        try {
            f(args...);
            return false;
        }
        catch (...) {
            return true;
        }
    };
}

template<typename Function>
auto invert(Function &f) {
    return [&f](auto... args) -> decltype(f(args...)) {
        return !f(args...);
    };
}

void _can_print() {
    static int i = 0;
    i++;
    if (i == 5)
        throw std::exception();

    std::cout << "I == " << i << std::endl;
}

auto can_print = never_throw(_can_print);
auto can_print_throws = did_throw(_can_print);
auto can_print_without_throwing = invert(can_print_throws);

void exec() {
    while (true)
        if (!can_print_without_throwing()) {
            std::cerr << "Something bad happened" << std::endl;
            break;
        }
}