#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

using IpAddress = std::array<int, 4>;

IpAddress parse_ip(const std::string &ip_text)
{
    IpAddress ip = {0, 0, 0, 0};
    std::vector<std::string> parts = split(ip_text, '.');
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

void print_ip(const IpAddress &ip)
{
    for (std::size_t i = 0; i < ip.size(); ++i)
    {
        if (i > 0)
        {
            std::cout << ".";
        }
        std::cout << ip[i];
    }
    std::cout << std::endl;
}

int main()
{
    try
    {
        std::vector<IpAddress> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(parse_ip(v.at(0)));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), [](const IpAddress &lhs, const IpAddress &rhs) {
            return lhs > rhs;
        });

        for (const auto &ip : ip_pool)
        {
            print_ip(ip);
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        for (const auto &ip : ip_pool)
        {
            if (ip[0] == 1)
            {
                print_ip(ip);
            }
        }

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        for (const auto &ip : ip_pool)
        {
            if (ip[0] == 46 && ip[1] == 70)
            {
                print_ip(ip);
            }
        }

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        for (const auto &ip : ip_pool)
        {
            if (std::any_of(ip.begin(), ip.end(), [](int part) { return part == 46; }))
            {
                print_ip(ip);
            }
        }

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
