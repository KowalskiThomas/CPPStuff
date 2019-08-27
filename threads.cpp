#include <iostream>
#include <thread>
#include <ostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <numeric>
#include <mutex>
#include <atomic>

std::mutex m1;
std::mutex m2;

struct Test
{
    int var = 0;
    auto test() -> void
    {
        var += 100;
    }

};


Test test;
std::atomic<Test> test_at;
volatile bool ready = false;

void call_from_thread(int x)
{
    for(int i = 0; i < 1000; i++) {
        // m1.lock();
        test.test();
        auto temp = test_at.load();
        auto before = temp;
        test_at.compare_exchange_strong(before, temp);
        ready = true;
    }
}

int main()
{
    std::vector<std::thread> threads;

    std::vector<int> values { 1, 2, 4, 8, 16 };

    std::for_each(values.begin(), values.end(), [&threads](const int x) {
       threads.emplace_back(std::bind(call_from_thread, x));
    });

    std::for_each(threads.begin(), threads.end(), [](std::thread& t) {
       t.join();
    });

    while (!ready)
    {
        ; // Do nothing
    }

    std::cout << test.var << ' ' << test_at.load().var << std::endl;

    return 0;
}