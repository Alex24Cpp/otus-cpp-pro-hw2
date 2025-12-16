#include "filtering_ip.hpp"

#include <algorithm>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "ip_filter.h"

IP_VECTOR filterAny(const IP_VECTOR &ip_pool, const Octet &octet) {
    IP_VECTOR filtered_ip;

    const std::string value_octet = std::visit(
        [](auto &&value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return value;
            } else if constexpr (std::is_same_v<T, const char *>) {
                return std::string(value);
            } else {
                return std::to_string(value);
            }
        },
        octet);

    for (const auto &ip_address : ip_pool) {
        if (std::find(ip_address.begin(), ip_address.end(), value_octet) !=
            ip_address.end()) {
            filtered_ip.emplace_back(ip_address);
        }
    }
    return filtered_ip;
}
