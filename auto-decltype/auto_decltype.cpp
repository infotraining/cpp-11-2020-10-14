#include "catch.hpp"
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <set>

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

    stream << 1 << "\n";
}

TEST_CASE("decltype")
{
    std::map<std::string, float> math_constants = { {"pi", 3.14f}, { "e", 2.71f } };

    auto a_map = math_constants;
    decltype(math_constants) d_map;

    REQUIRE(d_map.size() == 0);
    static_assert(
        std::is_same<std::map<std::string, float>, decltype(d_map)>::value, "Error");

    using RefItemT = decltype(d_map[std::declval<std::string>()]);  
    REQUIRE(d_map.size() == 0);  
}

template <typename T>
auto multiply(const T& a, const T& b) -> decltype(a * b)
{
    return a * b;
}

struct Vector2D
{
    double x, y;
};

// scalar multiplication
double operator*(const Vector2D& a, const Vector2D& b)
{
    return a.x * b.x + a.y * b.y;
}

TEST_CASE("multiply")
{
    Vector2D v1{1.0, 2.0};
    Vector2D v2{2.0, 3.0};

    REQUIRE(multiply(v1, v2) == Approx(8.0));
}

TEST_CASE("set")
{
    int x = 10;
    int y = 20;

    auto compare_by_value = [](int* a, int* b) { return *a < *b; };

    std::set<int*, decltype(compare_by_value)> pointers_set({&x, &y}, compare_by_value);

    for(const auto& ptr : pointers_set)
    {
        std::cout << ptr << " - " << *ptr << "\n";
    }
}

TEST_CASE("Modern C++")
{
    auto i1 = 10;
    int i2 = 10;    
}

template <typename T>
auto add(const T& a, const T& b) 
{
    return a + b;
}

auto describe(int value)
{
    if (value % 2 == 0)
        return "even"s;
    else
        return "odd"s;
}

auto factorial(int n)
{
    if (n == 1)
        return 1;
    return factorial(n-1) * n;
}    

TEST_CASE("new syntax for function")
{
    auto lambda = [](int a, int b) -> int { return a + b; };
}

std::map<int, std::string> dictionary = { {1, "one"}, {2, "two"} };

auto get_mapped_value(decltype(dictionary)& m, decltype(dictionary)::key_type key)
{
    return m.at(key);
}

decltype(auto) get_mapped_ref(std::map<int, std::string>& m, int key)
{
    return m.at(key);
}

TEST_CASE("auto vs. decltype(auto)")
{    
    get_mapped_ref(dictionary, 1).append("!");

    REQUIRE(dictionary[1] == "one!");
}