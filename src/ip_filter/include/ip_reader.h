#pragma once

#include <istream>
#include <vector>

#include "ip_address.h"

std::vector<IpAddress> read_ip_pool(std::istream &input);
