#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/tail/key_range.hpp>
#include <ecosnail/tail/value_range.hpp>

#include <map>

using ecosnail::tail::keyRange;
using ecosnail::tail::valueRange;

TEST_CASE("Iterate ranges", "[range]")
{
    const std::map<int, int> map {
        {1, 10},
        {2, 20},
        {3, 30},
    };

    SECTION("Iterate keys by const ref")
    {
        std::vector<int> keys;
        for (const auto& key : keyRange(map)) {
            keys.push_back(key);
        }
        REQUIRE(keys.size() == 3);
        REQUIRE(keys.at(0) == 1);
        REQUIRE(keys.at(1) == 2);
        REQUIRE(keys.at(2) == 3);
    }

    SECTION("Iterate values by const ref")
    {
        std::vector<int> values;
        for (const auto& value : valueRange(map)) {
            values.push_back(value);
        }
        REQUIRE(values.size() == 3);
        REQUIRE(values.at(0) == 10);
        REQUIRE(values.at(1) == 20);
        REQUIRE(values.at(2) == 30);
    }
}

TEST_CASE("Modify values", "[modify]")
{
    std::map<int, int> map {
        {1, 10},
        {2, 20},
        {3, 30},
    };

    for (auto& value : valueRange(map)) {
        value++;
    }

    REQUIRE(map.size() == 3);
    REQUIRE(map.at(1) == 11);
    REQUIRE(map.at(2) == 21);
    REQUIRE(map.at(3) == 31);
}
