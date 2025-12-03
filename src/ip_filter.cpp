#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "lib.hpp"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
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
    std::cout << "Start ip_filter. Version: " << Version() << '\n';

    try {
        std::vector<std::vector<std::string>> ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            auto sub_line = split(line, '\t');
            if (!sub_line.empty()) {
                ip_pool.emplace_back(split(sub_line.at(0), '.'));
            }
        }

        // TODO reverse lexicographically sort

        printIP(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

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
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
