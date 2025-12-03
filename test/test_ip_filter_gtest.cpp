#include <gtest/gtest.h>

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "filtering_ip.hpp"
#include "ip_filter.hpp"
#include "lib.hpp"
#include "sort_ip.hpp"

// ============= Тесты для функции split =============
struct SplitTest : public ::testing::Test {
    static std::vector<std::string> split(const std::string_view &str,
                                          char delim) {
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
};

TEST_F(SplitTest, SplitIP) {
    auto result = split("192.168.1.1", '.');
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "192");
    EXPECT_EQ(result[1], "168");
    EXPECT_EQ(result[2], "1");
    EXPECT_EQ(result[3], "1");
}

TEST_F(SplitTest, SplitSingleElement) {
    auto result = split("192", '.');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "192");
}

TEST_F(SplitTest, SplitWithTab) {
    auto result = split("192.168.1.1\tother_data", '\t');
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "192.168.1.1");
    EXPECT_EQ(result[1], "other_data");
}

TEST_F(SplitTest, SplitEmptyString) {
    auto result = split("", '.');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "");
}

// ============= Тесты для функции filter =============
struct FilterTest : public ::testing::Test {
    IP_VECTOR pool{{"192", "168", "1", "1"}, {"192", "168", "1", "2"},
                   {"192", "168", "2", "1"}, {"10", "0", "0", "1"},
                   {"10", "1", "0", "1"},    {"172", "16", "0", "1"}};
};

TEST_F(FilterTest, FilterByFirstOctet) {
    auto result = filter(pool, "192");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0][0], "192");
    EXPECT_EQ(result[1][0], "192");
    EXPECT_EQ(result[2][0], "192");
}

TEST_F(FilterTest, FilterByFirstAndSecondOctet) {
    auto result = filter(pool, "192", "168");
    ASSERT_EQ(result.size(), 3);
    for (const auto &ip_addr : result) {
        EXPECT_EQ(ip_addr[0], "192");
        EXPECT_EQ(ip_addr[1], "168");
    }
}

TEST_F(FilterTest, FilterByFirstOctetNoResults) {
    // auto pool = createTestPool();
    auto result = filter(pool, "1");
    ASSERT_EQ(result.size(), 0);
}

TEST_F(FilterTest, FilterEmptyPool) {
    const IP_VECTOR pool;
    auto result = filter(pool, "192");
    ASSERT_EQ(result.size(), 0);
}

// ============= Тесты для функции filter_any =============
struct FilterAnyTest : public ::testing::Test {
    IP_VECTOR pool{{"192", "168", "1", "1"},
                   {"192", "168", "1", "2"},
                   {"10", "168", "2", "1"},
                   {"10", "0", "0", "1"},
                   {"172", "16", "168", "5"}};
};

TEST_F(FilterAnyTest, FilterAnyByOctet) {
    auto result = filter_any(pool, "168");
    ASSERT_EQ(result.size(), 4);
}

TEST_F(FilterAnyTest, FilterAnyNoResults) {
    auto result = filter_any(pool, "99");
    ASSERT_EQ(result.size(), 0);
}

TEST_F(FilterAnyTest, FilterAnyEmptyPool) {
    const IP_VECTOR pool;
    auto result = filter_any(pool, "192");
    ASSERT_EQ(result.size(), 0);
}

TEST_F(FilterAnyTest, FilterAnySingleMatch) {
    auto result = filter_any(pool, "172");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "172");
}

TEST_F(FilterAnyTest, FilterAnyMultipleOctets) {
    auto result = filter_any(pool, "1");
    ASSERT_EQ(result.size(), 4);  // 1 появляется в нескольких октетах
}

// ============= Тесты для функции reversSort =============
struct ReversSortTest : public ::testing::Test {
protected:
    IP_VECTOR verification_pool = {
        {"193", "136", "199", "50"}, {"193", "93", "192", "192"},
        {"193", "93", "192", "192"}, {"193", "93", "192", "165"},
        {"193", "93", "192", "134"}, {"193", "93", "192", "122"},
        {"193", "78", "85", "148"},  {"5", "101", "219", "197"},
        {"5", "101", "219", "107"},  {"5", "101", "217", "197"},
        {"5", "8", "47", "202"},     {"5", "8", "47", "100"},
        {"5", "8", "47", "84"},      {"5", "8", "4", "44"},
        {"2", "29", "170", "242"}};
};

TEST_F(ReversSortTest, SortUnsortedPool) {
    IP_VECTOR pool = {{"193", "136", "199", "50"}, {"2", "29", "170", "242"},
                      {"5", "8", "47", "100"},     {"193", "93", "192", "165"},
                      {"5", "8", "47", "84"},      {"193", "93", "192", "134"},
                      {"5", "8", "47", "202"},     {"5", "101", "219", "107"},
                      {"5", "8", "4", "44"},       {"193", "93", "192", "192"},
                      {"193", "78", "85", "148"},  {"5", "101", "219", "197"},
                      {"193", "93", "192", "122"}, {"5", "101", "217", "197"},
                      {"193", "93", "192", "192"}};
    reversSort(pool);
    ASSERT_EQ(verification_pool, pool);
}

TEST_F(ReversSortTest, SortAlreadySorted) {
    IP_VECTOR pool = {{"193", "136", "199", "50"}, {"193", "93", "192", "192"},
                      {"193", "93", "192", "192"}, {"193", "93", "192", "165"},
                      {"193", "93", "192", "134"}, {"193", "93", "192", "122"},
                      {"193", "78", "85", "148"},  {"5", "101", "219", "197"},
                      {"5", "101", "219", "107"},  {"5", "101", "217", "197"},
                      {"5", "8", "47", "202"},     {"5", "8", "47", "100"},
                      {"5", "8", "47", "84"},      {"5", "8", "4", "44"},
                      {"2", "29", "170", "242"}};

    reversSort(pool);
    ASSERT_EQ(verification_pool, pool);
}

TEST_F(ReversSortTest, SortSingleElement) {
    IP_VECTOR pool = {{"192", "168", "1", "1"}};
    reversSort(pool);
    ASSERT_EQ(pool.size(), 1);
    EXPECT_EQ(pool[0][0], "192");
}

TEST_F(ReversSortTest, SortEmptyPool) {
    IP_VECTOR pool;
    reversSort(pool);
    ASSERT_EQ(pool.size(), 0);
}

// ============= Тест версии =============
TEST(Version, Positive) {
    ASSERT_GT(Version(), 0);
}