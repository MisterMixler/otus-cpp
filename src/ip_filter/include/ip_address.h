#pragma once

#include <array>
#include <iosfwd>
#include <string>

using IpAddress = std::array<int, 4>;

IpAddress parse_ip(const std::string &ip_text);
std::ostream &operator<<(std::ostream &os, const IpAddress &ip);
