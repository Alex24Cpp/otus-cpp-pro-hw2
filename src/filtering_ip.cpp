#include "filtering_ip.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "ip_filter.h"

IP_VECTOR filterAny(const IP_VECTOR &ip_pool, const std::string &octet) {
    IP_VECTOR filtered_ip;
    for (const auto &ip_address : ip_pool) {
        if (std::find(ip_address.begin(), ip_address.end(), octet) !=
            ip_address.end()) {
            filtered_ip.emplace_back(ip_address);
        }
    }
    return filtered_ip;
}
