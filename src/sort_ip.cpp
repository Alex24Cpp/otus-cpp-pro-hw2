#include "sort_ip.h"

#include <algorithm>
#include <string>
#include <vector>

#include "ip_filter.h"

void reversSort(IP_VECTOR &ip_pool) {
    std::sort(
        ip_pool.begin(), ip_pool.end(),
        [](const std::vector<std::string> &addr_1,
           const std::vector<std::string> &addr_2) {
            if (std::stoi(addr_1.at(0)) == std::stoi(addr_2.at(0))) {
                if (std::stoi(addr_1.at(1)) == std::stoi(addr_2.at(1))) {
                    if (std::stoi(addr_1.at(2)) == std::stoi(addr_2.at(2))) {
                        return std::stoi(addr_1.at(3)) >
                               std::stoi(addr_2.at(3));
                    }
                    return std::stoi(addr_1.at(2)) > std::stoi(addr_2.at(2));
                }
                return std::stoi(addr_1.at(1)) > std::stoi(addr_2.at(1));
            }
            return std::stoi(addr_1.at(0)) > std::stoi(addr_2.at(0));
        });
}
