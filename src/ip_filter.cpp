#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "filtering_ip.hpp"
// #include "lib.hpp"
#include "ip_filter.hpp"
#include "sort_ip.hpp"

std::vector<std::string> split(const std::string_view &str, char delim) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t stop = str.find(delim);
    while (stop != std::string_view::npos) {
        result.emplace_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find(delim, start);
    }
    result.emplace_back(str.substr(start));
    return result;
}

void printIP(const std::vector<std::vector<std::string>> &ip_pool) {
    if (!ip_pool.empty()) {
        for (const auto &ip_addr : ip_pool) {
            bool first = true;
            for (const auto &ip_part : ip_addr) {
                if (!first) {
                    std::cout << ".";
                }
                std::cout << ip_part;
                first = false;
            }
            std::cout << '\n';
        }
    } else {
        std::cout << "No data!" << '\n';
    }
}

int main() {
    // std::cout << "Start ip_filter. Version: " << Version() << '\n';

    try {
        IP_VECTOR ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            auto sub_line = split(line, '\t');
            if (!sub_line.empty()) {
                ip_pool.emplace_back(split(sub_line.at(0), '.'));
            }
        }

        reversSort(ip_pool);
        printIP(ip_pool);

        IP_VECTOR ip_pool_filtered = filter(ip_pool, "1");
        printIP(ip_pool_filtered);

        ip_pool_filtered = filter(ip_pool, "46", "70");
        printIP(ip_pool_filtered);

        ip_pool_filtered = filter_any(ip_pool, "46");
        printIP(ip_pool_filtered);

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
