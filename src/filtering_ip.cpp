#include "filtering_ip.hpp"

#include <string>
#include <vector>

#include "ip_filter.h"

IP_VECTOR filter_any(const IP_VECTOR &ip_pool, const std::string &octet) {
    IP_VECTOR filtered_ip;
    for (const auto &ip_address : ip_pool) {
        if (ip_address.at(0) == octet || ip_address.at(1) == octet ||
            ip_address.at(2) == octet || ip_address.at(3) == octet) {
            filtered_ip.emplace_back(ip_address);
        }
    }
    return filtered_ip;
}
