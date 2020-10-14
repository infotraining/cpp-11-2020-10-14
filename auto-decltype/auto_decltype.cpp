#include "catch.hpp"
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("auto")
{
    auto i = 42; // int
    const auto pi(3.14); // const double
    auto txt = "text"; // const char*
    auto str = "text"s; // std::string
    auto& ref_str = str; // std::string&
    auto* ptr_str = &str; // std::string*

    vector<vector<int>> matrix;

    //...

    for (auto it = matrix.begin(); it != matrix.end(); it++)
    {
        std::cout << it->size() << "\n";
    }

    constexpr auto e = 2.71;
}

TEST_CASE("iteration over container")
{
    vector<int> vec = {1, 2, 3};

    for (auto it = vec.begin(); it != vec.end(); ++it)
        (*it) *= 2;

    for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    REQUIRE(vec == vector<int> {2, 4, 6});
}

template <typename T>
void deduce1(T obj)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce2(T& obj)
{
    puts(__PRETTY_FUNCTION__);
}

int foo(int arg)
{
    return arg;
}

TEST_CASE("auto type deduction - case 1")
{
    int x = 10;
    const int cx = 42;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    deduce1(x); // T = int
    auto ax1 = x; // int

    deduce1(cx); // T = int
    auto ax2 = cx; // int

    deduce1(ref_x); // T = int
    auto ax3 = ref_x; // int

    deduce1(cref_x); // T = int
    auto ax4 = cref_x; // int

    deduce1(tab); // T = int*
    auto ax5 = tab; // int*

    deduce1(foo); // T = int(*)(int)
    auto ax6 = foo; // int(*)(int)
}

TEST_CASE("auto type deduction - case 2")
{
    int x = 10;
    const int cx = 42;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    deduce2(x); // int&
    auto& ax1 = x; // int&

    deduce2(cx);
    auto& ax2 = cx; // const int&

    deduce2(ref_x);
    auto& ax3 = ref_x; // int&

    deduce2(cref_x);
    auto& ax4 = cref_x; // const int&

    deduce2(tab);
    auto& ax5 = tab; // int(&)[10]

    deduce2(foo);
    auto& ax6 = foo; // int(&)(int)
}

TEST_CASE("auto declaration syntax")
{
    auto x1 = 1; // int
    auto ax1(1); // int

    SECTION("before C++17 - C++11/14")
    {
        auto a1 = 1; // int
        auto a2{1};  // std::initializer_list<int>
                
        auto a3 = {1}; // std::initializer_list<int>
        auto a4 = {1, 2, 3}; // std::initializer_list<int>
    }

    SECTION("before C++17")
    {
        auto a1 = 1; // int
        auto a2{1};  // int
        //auto a3{1, 2, 3}; // ill-formed - ERROR
        
        auto a3 = {1}; // std::initializer_list<int>
        auto a4 = {1, 2, 3}; // std::initializer_list<int>
    }
}

TEST_CASE("streams")
{
    auto& stream = std::cout;
}