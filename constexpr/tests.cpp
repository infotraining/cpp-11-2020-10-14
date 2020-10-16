#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;
using namespace Catch::Matchers;

constexpr int id = 665;
constexpr int next_id = id + 1;

const int size_m = 1024;
constexpr int size_l = size_m * 2;

// int x = 5;
// constexpr auto value = x; // ERROR

namespace Cpp11
{
    constexpr int factorial(int n)
    {
        return (n == 0) ? 1 : n * factorial(n - 1);
    }
}

template <size_t N>
struct Factorial
{
    const static int value = N * Factorial<N-1>::value;
};

template <>
struct Factorial<0>
{
    const static int value = 1;
};


constexpr int factorial(int n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

TEST_CASE("constexpr")
{
    int tab[factorial(4)];

    std::cout << sizeof(tab) << "\n";
}

template <int N>
constexpr std::array<int, N> create_factorial_lookup()
{
    std::array<int, N> result = {};

    for(int i = 0; i < N; ++i)
    {
        result[i] = factorial(i);
    }

    return result;
}

TEST_CASE("constexpr functions")
{
    constexpr size_t size = 5;
    constexpr std::array<int, size> factorial_lookup = create_factorial_lookup<size>();

    static_assert(factorial_lookup[0] == 1, "Error");
    static_assert(factorial_lookup[4] == 24, "Error");

    auto numbers = create_factorial_lookup<10>(); 
    numbers[0] = 665;
}