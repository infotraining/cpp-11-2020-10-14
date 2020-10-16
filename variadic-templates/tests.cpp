#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

void print()
{
    std::cout << "\n";
}

template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail)
{
    std::cout << head << " ";
    print(tail...);
}

namespace Cpp17
{
    template <typename Head, typename... Tail>
    void print(const Head& head, const Tail&... tail)
    {
        puts(__PRETTY_FUNCTION__);

        std::cout << head << "\n";

        if constexpr(sizeof...(tail) > 0)
            print(tail...);
        else
            std::cout << "\n";        
    }
}

TEST_CASE("variadic function")
{
    Cpp17::print(1, 3.14, 5.9f, "abc", 1, 3.14, "text", "abc");
    print(1, 3.14);
}

template <typename... Args>
struct ListType
{
    constexpr static size_t size = sizeof...(Args);
};

TEST_CASE("list of types")
{
    static_assert(ListType<int, double>::size == 2, "Error"); 
}
