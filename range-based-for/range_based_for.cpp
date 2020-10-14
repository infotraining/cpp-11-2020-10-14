#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

TEST_CASE("range based for container")
{
    std::vector<int> vec = {1, 2, 3};

    int sum = 0;
    for(int item : vec)
    {
        sum += item;
    }

    REQUIRE(sum == 6);
    
    SECTION("is interpreted as")
    {
        for(auto it = vec.begin(); it != vec.end(); ++it)
        {
           int item = *it; 
           sum += item;
        }
    }
}

TEST_CASE("range based for for native array")
{
    int tab[] = {1, 2, 3};

    int sum = 0;
    for(int item : tab)
    {
        sum += item;
    }

    REQUIRE(sum == 6);

    SECTION("is interpreted as")
    {
        for(auto it = begin(tab); it != end(tab); ++it)
        {
           int item = *it; 
           sum += item;
        }
    }
}

TEST_CASE("range based for for initializer lists")
{
    for(auto item : { "one", "two", "three" })
    {
        std::cout << item << "\n";
    }
}

TEST_CASE("effective usage of range based for")
{
    std::vector<std::string> words = { "one", "two", "three" };

    for(auto& w : words)
    {
        w.append("!");
    }

    for(const auto& w : words)
    {
        std::cout << w << "\n";
    }

    SECTION("is interpreted as")
    {
        for(auto it = words.begin(); it != words.end(); ++it)
        {
           const auto& w = *it; 
           std::cout << w << "\n";
        }
    }
}


struct Points
{
    std::vector<std::tuple<int, int>> pts;

    // auto begin()
    // {
    //     return pts.begin();
    // }

    // auto end()
    // {
    //     return pts.end();
    // }

    // auto begin() const
    // {
    //     return pts.begin();
    // }

    // auto end() const
    // {
    //     return pts.end();
    // }
};

auto begin(Points& p)
{
    return p.pts.begin();
}

auto end(Points& p)
{
    return p.pts.begin();
}

auto begin(const Points& p)
{
    return p.pts.begin();
}

auto end(const Points& p)
{
    return p.pts.begin();
}

TEST_CASE("custom objects")
{
    const Points pts { { {1, 2}, {3, 4}, {5, 6} } };

    for(const auto& pt : pts)
    {
        std::cout << std::get<0>(pt) << " " << std::get<1>(pt) << "\n";
    }
}