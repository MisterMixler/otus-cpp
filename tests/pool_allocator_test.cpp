#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

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

TEST(PoolAllocatorTest, Requirements) {
    // std::map<int, int> with default allocator
    std::map<int, int> default_map;
    for (int i = 0; i < 10; ++i) {
        default_map.emplace(i, factorial(i));
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(default_map.at(i), factorial(i));
    }

    // std::map<int, int> with custom allocator limited to 10 elements
    using MapAllocator = PoolAllocator<std::pair<const int, int>, 10>;
    std::map<int, int, std::less<int>, MapAllocator> custom_map;
    for (int i = 0; i < 10; ++i) {
        custom_map.emplace(i, factorial(i));
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(custom_map.at(i), factorial(i));
    }

    // Capture stdout for required printing
    testing::internal::CaptureStdout();

    // Print all values stored in map: "key value"
    for (const auto& item : custom_map) {
        std::cout << item.first << ' ' << item.second << '\n';
    }

    // SimpleContainer<int> with default allocator
    SimpleContainer<int> default_container;
    for (int i = 0; i < 10; ++i) {
        default_container.push_back(i);
    }
    int expected = 0;
    for (const auto& value : default_container) {
        EXPECT_EQ(value, expected++);
    }

    // SimpleContainer<int> with custom allocator limited to 10 elements
    SimpleContainer<int, PoolAllocator<int, 10>> custom_container;
    for (int i = 0; i < 10; ++i) {
        custom_container.push_back(i);
    }
    expected = 0;
    for (const auto& value : custom_container) {
        EXPECT_EQ(value, expected++);
    }

    // Print all values stored in container
    for (const auto& value : custom_container) {
        std::cout << value << '\n';
    }

    const std::string out = testing::internal::GetCapturedStdout();

    std::ostringstream expected_out;
    for (int i = 0; i < 10; ++i) {
        expected_out << i << ' ' << factorial(i) << '\n';
    }
    for (int i = 0; i < 10; ++i) {
        expected_out << i << '\n';
    }

    EXPECT_EQ(out, expected_out.str());
}