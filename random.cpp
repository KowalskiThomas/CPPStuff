#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>
#include <random>
#include <map>

constexpr const bool show_output = false;

template<typename generator, size_t roll_count>
void test1()
{
    auto seed = 123;
    auto rng = generator{};
    auto distribution = std::uniform_int_distribution(static_cast<size_t>(0), static_cast<size_t>(1023));

    if (show_output)
    {
        std::cout << "Raw output" << std::endl;
        for(auto i = 0; i < 10; i++)
            std::cout << rng() << ' ';
        std::cout << std::endl;

        std::cout << "From uniform int distribution (0, 1023)" << std::endl;
        for (auto i = 0; i < 10; i++)
            std::cout << distribution(rng) << ' ';
        std::cout << std::endl;
    }

    std::map<size_t, size_t> histogram;
    for(size_t i = 0; i < roll_count; i++)
        histogram[distribution(rng)]++;

    size_t min = 0 - 1;
    size_t max = 0;
    for(auto pair : histogram)
    {
        if (pair.second < min)
            min = pair.second;
        if (pair.second > max)
            max = pair.second;
    }

    std::cout
        << "Key count: " << histogram.size()
        << " Min count: " << min
        << " Max count: " << max
        << std::endl;
}

void exec();

int main()
{
    exec();
    return 0;
}

void exec()
{
    using output = long long;
    constexpr const size_t roll_count = (1ull << 21u);

    using congruential_rng = std::linear_congruential_engine<output, 123, 42, 1024>;
    std::cout << "Congruential" << std::endl;
    test1<congruential_rng, roll_count>();

    using mersenne_19937 = std::mt19937;
    std::cout << "Mersenne 19937" << std::endl;
    test1<mersenne_19937, roll_count>();
}