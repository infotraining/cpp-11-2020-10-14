#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

TEST_CASE("variadic function")
{
    print(1, 3.14, "text"s, "abc", 1, 3.14, "text"s, "abc");
    print(1, 3.14);
}