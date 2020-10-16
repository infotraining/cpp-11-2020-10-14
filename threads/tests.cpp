#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;
using namespace Catch::Matchers;

void background_work(size_t id, const std::string& text, std::chrono::milliseconds delay)
{
    std::cout << "bw#" << id << " has started..." << std::endl;

    for (const auto& c : text)
    {
        std::cout << "bw#" << id << ": " << c << std::endl;

        std::this_thread::sleep_for(delay);
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

int calculate_square(int x)
{
    cout << "Starting calculation for " << x << endl;

    int interval = rand() % 5000;

    this_thread::sleep_for(chrono::milliseconds(interval));

    if (x == 13)
        throw runtime_error("Error#13");

    return x * x;
}

TEST_CASE("Test")
{
    REQUIRE(1 == 1);
}