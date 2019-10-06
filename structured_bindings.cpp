#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>

auto make_tuple(int a, int b) -> std::pair<int, int> {
    return std::make_pair(a, b);
}

using std::get;

class my_array {
    template<int i>
    friend auto std::get(my_array&);
};

namespace std {
    template<> struct tuple_element<0, my_array> { using type = int; };
    template<> struct tuple_element<1, my_array> { using type = string; };
    template<> struct tuple_element<2, my_array> { using type = int; };
    template<> struct tuple_size<my_array>: public integral_constant<size_t, 3> {};
}

template<int i>
constexpr auto get(const my_array&) {
    if constexpr (i == 0)
        return 30;
    else if constexpr (i == 1)
        return std::string{"coucou"};
    else
        return 42;
}

void exec();

int main() {
    exec();
    return 0;
}

void exec() {
    // auto[a, b] = make_tuple(1, 2);
    // [a, b] = make_tuple(3, 4);

    my_array test;
    auto [integer_2, string, integer] = test;
    std::cout << string << integer_2 << integer << std::end;
}

