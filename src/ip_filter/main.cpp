#include "ip_address.h"
#include "ip_filter.h"
#include "ip_reader.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        std::vector<IpAddress> ip_pool = read_ip_pool(std::cin);

        std::sort(ip_pool.begin(), ip_pool.end(),
                  [](const IpAddress &lhs, const IpAddress &rhs) { return lhs > rhs; });

        for (const auto &ip : ip_pool)
        {
            std::cout << ip << '\n';
        }

        for (const auto &ip : filter_by_prefix(ip_pool, 1))
        {
            std::cout << ip << '\n';
        }

        for (const auto &ip : filter_by_prefix(ip_pool, 46, 70))
        {
            std::cout << ip << '\n';
        }

        for (const auto &ip : filter_by_any(ip_pool, 46))
        {
            std::cout << ip << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
