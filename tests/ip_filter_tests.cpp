#include "ip_address.h"
#include "ip_filter.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <sstream>

TEST(IpAddressTest, ParseValidAndOutput)
{
    const IpAddress ip = parse_ip("1.2.3.4");
    EXPECT_EQ(ip, (IpAddress{1, 2, 3, 4}));

    std::ostringstream out;
    out << ip;
    EXPECT_EQ(out.str(), "1.2.3.4");
}

TEST(IpAddressTest, ParseInvalidOctetCount)
{
    EXPECT_THROW(parse_ip("10.0.1"), std::runtime_error);
}

TEST(IpFilterTest, FiltersByPrefixAndAny)
{
    const std::vector<IpAddress> pool = {
        {1, 2, 3, 4},
        {1, 2, 5, 6},
        {2, 1, 1, 1},
        {46, 70, 0, 1},
    };

    const auto prefix = filter_by_prefix(pool, 1, 2);
    ASSERT_EQ(prefix.size(), 2U);
    EXPECT_EQ(prefix[0], (IpAddress{1, 2, 3, 4}));
    EXPECT_EQ(prefix[1], (IpAddress{1, 2, 5, 6}));

    const auto any = filter_by_any(pool, 46);
    ASSERT_EQ(any.size(), 1U);
    EXPECT_EQ(any[0], (IpAddress{46, 70, 0, 1}));
}
