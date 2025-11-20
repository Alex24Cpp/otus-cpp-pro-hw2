#include <gtest/gtest.h>

#include <lib.hpp>

TEST(Version, Pozitive) {
    ASSERT_GT(version(), 0);
}