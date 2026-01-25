#include "ip_filter.h"

#include <algorithm>

std::vector<IpAddress> filter_by_prefix(const std::vector<IpAddress> &pool,
                                        const std::vector<int> &prefix)
{
    if (prefix.empty())
    {
        return pool;
    }

    std::vector<IpAddress> result;
    result.reserve(pool.size());
    for (const auto &ip : pool)
    {
        if (prefix.size() > ip.size())
        {
            continue;
        }

        bool matches = true;
        for (std::size_t i = 0; i < prefix.size(); ++i)
        {
            if (ip[i] != prefix[i])
            {
                matches = false;
                break;
            }
        }

        if (matches)
        {
            result.push_back(ip);
        }
    }
    return result;
}

std::vector<IpAddress> filter_by_any(const std::vector<IpAddress> &pool, int value)
{
    std::vector<IpAddress> result;
    result.reserve(pool.size());
    for (const auto &ip : pool)
    {
        if (std::any_of(ip.begin(), ip.end(), [value](int part) { return part == value; }))
        {
            result.push_back(ip);
        }
    }
    return result;
}
