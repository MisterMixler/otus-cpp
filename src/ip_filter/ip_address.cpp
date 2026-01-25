#include "ip_address.h"

#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(delimiter);
    while (stop != std::string::npos)
    {
        result.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}
} // namespace

IpAddress parse_ip(const std::string &ip_text)
{
    IpAddress ip = {0, 0, 0, 0};
    const std::vector<std::string> parts = split(ip_text, '.');
    if (parts.size() != ip.size())
    {
        throw std::runtime_error("Invalid IP address: " + ip_text);
    }
    for (std::size_t i = 0; i < ip.size(); ++i)
    {
        ip[i] = std::stoi(parts[i]);
    }
    return ip;
}

std::ostream &operator<<(std::ostream &os, const IpAddress &ip)
{
    for (std::size_t i = 0; i < ip.size(); ++i)
    {
        if (i > 0)
        {
            os << '.';
        }
        os << ip[i];
    }
    return os;
}
