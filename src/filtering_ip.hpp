#pragma once

#include <string>

#include "ip_filter.hpp"

IP_VECTOR filter(const IP_VECTOR &ip_pool, const std::string &octet_1,
                 const std::string &octet_2 = "");

IP_VECTOR filter_any(const IP_VECTOR &ip_pool, const std::string &octet);