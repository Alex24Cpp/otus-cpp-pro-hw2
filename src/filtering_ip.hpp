#pragma once

#include <string>
#include <vector>

#include "ip_filter.h"

IP_VECTOR filter_any(const IP_VECTOR &ip_pool, const std::string &octet);

template <typename... Octets>
IP_VECTOR filter(const IP_VECTOR &ip_pool, Octets... octets) {
    IP_VECTOR filtered_ip;

    for (const auto &ip_address : ip_pool) {
        bool match = true;
        std::size_t index = 0;
        // fold expression: проверка каждого октета
        ((match = match && (ip_address.at(index++) == octets)), ...);
        if (match) {
            filtered_ip.emplace_back(ip_address);
        }
    }

    return filtered_ip;
}
