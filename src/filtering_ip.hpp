#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "ip_filter.h"

IP_VECTOR filterAny(const IP_VECTOR &ip_pool, const std::string &octet);

template <typename... Octets>
IP_VECTOR filter(const IP_VECTOR &ip_pool, Octets... octets) {
    constexpr std::size_t nunberOfParam = sizeof...(octets);
    IP_VECTOR filtered_ip;

    // Если ip_pool пустой — вернуть пустой результат
    if (ip_pool.empty()) {
        return {};
    }

    if (ip_pool.front().size() < nunberOfParam) {
        throw std::invalid_argument(
            "filter(): слишком много аргументов (октетов) передано (" +
            std::to_string(nunberOfParam) + "), должно быть не более " +
            std::to_string(ip_pool.front().size()));
    }

    for (const auto &ip_address : ip_pool) {
        bool match = true;
        std::size_t index = 0;
        // fold expression: проверка каждого октета
        ((match = match && (ip_address[index++] == octets)), ...);
        if (match) {
            filtered_ip.emplace_back(ip_address);
        }
    }

    return filtered_ip;
}
