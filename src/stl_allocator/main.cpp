#include <iostream>
#include <map>
#include <utility>

#include "simple_container.h"
#include "stl_allocator.h"

namespace {

int factorial(int value) {
    int result = 1;
    for (int i = 1; i <= value; ++i) {
        result *= i;
    }
    return result;
}

}  // namespace

int main() {
    std::map<int, int> default_map;
    for (int i = 0; i < 10; ++i) {
        default_map.emplace(i, factorial(i));
    }

    using MapAllocator = PoolAllocator<std::pair<const int, int>, 10>;
    std::map<int, int, std::less<int>, MapAllocator> custom_map;
    for (int i = 0; i < 10; ++i) {
        custom_map.emplace(i, factorial(i));
    }

    for (const auto& item : custom_map) {
        std::cout << item.first << ' ' << item.second << std::endl;
    }

    SimpleContainer<int> default_container;
    for (int i = 0; i < 10; ++i) {
        default_container.push_back(i);
    }

    SimpleContainer<int, PoolAllocator<int, 10>> custom_container;
    for (int i = 0; i < 10; ++i) {
        custom_container.push_back(i);
    }

    for (const auto& value : custom_container) {
        std::cout << value << std::endl;
    }

    return 0;
}