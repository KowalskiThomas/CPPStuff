#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>
#include <deque>
#include <map>

unsigned long get_timestamp()
{
    return time(nullptr);
}

int factorial(int x)
{
    int total = 1;
    while(x > 1) {
        total *= x;
        x--;
    }
    return total;
}

class lru_cache
{
    /*
     * Idea:
     *  - Hold a queue of things to evict, with a timestamp of last access.
     *    The least recently used one should be the first to be evicted.
     *  - Hold a map of last access times for each key. Use this map to check that the "eviction entry" from the queue
     *    is indeed the least recently used item.
     * When we want to insert something new, if the container is full, evict the oldest one. To do that:
     *  - Pop an element from the queue. Check if the popped timestamp matches the one in the map.
     *    - If it does not, then that means that this "eviction entry" was inserted before the last time we accessed
     *      the element. That means this element is not actually the least recently used.
     *      => Pop again, until we find an element that matches the timestamp in the map.
     *    - If it does, remove the cache entry for this key.
     *    - (Potentially, remove the map entry for this key)
     *  - Get the current timestamp, and
     *    - Enqueue a new "eviction entry" (key, timestamp) in the eviction queue
     *    - Update the value in the map: last_access[key] = timestamp
     *    - Add the element to the cache
     * When we want to check if an element is in the cache
     *  - Check whether data[key] exists
     *  - Update last_access[key]
     *  - Insert a new eviction entry (key, current timestamp)
     */

    using timestamp = unsigned long;
    using eviction_entry = std::pair<int, timestamp>;

    std::map<int, timestamp> last_access;
    std::deque<eviction_entry> to_evict;
    std::map<int, int> cached_data;

public:
    int get_value(int key)
    {
        return factorial(key);
    }

    int get(int key)
    {
        if (cached_data.find(key) != cached_data.end())
        {
            std::cout << key << " found in cache" << std::endl;
            auto current_timestamp = get_timestamp();
            last_access[key] = current_timestamp;
            to_evict.emplace_back(key, current_timestamp);
            return cached_data[key];
        }
        else
        {
            std::cout << key << " not in cache.";
            if (cached_data.size() >= 3) {
                auto evict_entry = evict_least_recently_used();
                std::cout << " Evicting " << evict_entry.first;
            }
            std::cout << " Inserting " << key << std::endl;
            insert(key, get_value(key));
            return cached_data[key];
        }
    }

    eviction_entry evict_least_recently_used()
    {
        eviction_entry entry = to_evict.front();
        to_evict.pop_front();
        while(entry.second != last_access[entry.first])
        {
            entry = to_evict.front();
            to_evict.pop_front();
        }

        last_access.erase(entry.first);
        cached_data.erase(entry.first);

        return entry;
    }

    void insert(int key, int value)
    {
        auto current_timestamp = get_timestamp();
        last_access[key] = current_timestamp;
        to_evict.emplace_back(key, current_timestamp);
        cached_data[key] = value;
    }
};

void exec();

int main()
{
    exec();
    return 0;
}

void exec()
{
    lru_cache cache;
    std::cout << cache.get(0) << std::endl;
    std::cout << cache.get(1) << std::endl;
    std::cout << cache.get(3) << std::endl;
    std::cout << cache.get(4) << std::endl;
    std::cout << cache.get(1) << std::endl;
    std::cout << cache.get(0) << std::endl;
}