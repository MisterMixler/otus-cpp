#include "ip_reader.h"

#include <string>
#include <vector>

std::vector<IpAddress> read_ip_pool(std::istream &input)
{
    std::vector<IpAddress> ip_pool;
    for (std::string line; std::getline(input, line);)
    {
        const std::string::size_type tab_pos = line.find('\t');
        const std::string ip_text =
            (tab_pos == std::string::npos) ? line : line.substr(0, tab_pos);
        ip_pool.push_back(parse_ip(ip_text));
    }
    return ip_pool;
}
