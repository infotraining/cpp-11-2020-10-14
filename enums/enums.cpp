#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

// forward declaration
enum Coffee : std::uint64_t;

void drink(Coffee c);

// type definition
enum Coffee : std::uint64_t
{
    Espresso,
    Cappucino,
    Late,
    Chemex = 1024
};

void drink(Coffee c)
{
    if (c == Chemex)
        std::cout << "Chemex\n";
}

TEST_CASE("classic enums")
{
    static_assert(sizeof(Coffee) == 8, "Error");
    std::cout << sizeof(Coffee) << "\n";
    static_assert(is_same<underlying_type_t<Coffee>,uint64_t>::value, "Error");

    std::underlying_type_t<Coffee> index = Chemex;
}

enum class Day : uint8_t { Mon = 1, Tue, Wed, Thd, Fri, Sat, Sun };

TEST_CASE("scoped enumerations")
{
    Day day_of_week = Day::Mon;

    REQUIRE(day_of_week == Day::Mon);

    auto index = static_cast<std::underlying_type_t<Day>>(Day::Thd);
    REQUIRE(index == 4);

    uint8_t number = 3;
    day_of_week = static_cast<Day>(number);

    REQUIRE(day_of_week == Day::Wed);
}

TEST_CASE("enums in C++17")
{
    Day day_of_week = Day::Sat;
    Day another_day{Day::Mon};
    Day weekend{7};
    // Day other_day = 7; // ERROR
}

enum class Byte : uint8_t {};

TEST_CASE("std::byte")
{
    std::byte bt{1};

    //byte = byte + 5;
    //byte = byte / 5;

    bt = (~bt) << 2;

    std::cout << "bt: " << to_integer<int>(bt) << "\n";
}