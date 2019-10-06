#include <iostream>
#include <vector>
#include <ostream>
#include <string>
#include <thread>
#include <chrono>

void exec();

int main()
{
    exec();
    return 0;
}

auto threaded_function() {
    for(int i = 0; i < 3000000; i++)
        ;
}

auto do_unthreaded()
{
    for(int i = 0; i < 100; i++)
        threaded_function();
}

auto do_threaded()
{
    std::vector<std::thread> threads;
    for(int i = 0; i < 100; i++)
    {
        threads.emplace_back(std::thread{ threaded_function });
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread& t) {
        t.join();
    });
}

template<typename Function>
auto get_execution_time(Function f)
{
    auto tick = std::chrono::system_clock::now();
    f();
    auto tock = std::chrono::system_clock::now();
    auto time = tock - tick;
    return std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}

void exec()
{
    std::cout << get_execution_time(do_unthreaded) << std::endl;
    std::cout << get_execution_time(do_threaded) << std::endl;
}