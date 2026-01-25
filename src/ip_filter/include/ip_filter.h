#pragma once

#include <vector>

#include "ip_address.h"

std::vector<IpAddress> filter_by_prefix(const std::vector<IpAddress> &pool,
                                        const std::vector<int> &prefix);

std::vector<IpAddress> filter_by_any(const std::vector<IpAddress> &pool, int value);

template <typename... Args>
std::vector<IpAddress> filter_by_prefix(const std::vector<IpAddress> &pool, Args... prefix)
{
    return filter_by_prefix(pool, std::vector<int>{static_cast<int>(prefix)...});
}
