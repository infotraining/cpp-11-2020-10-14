#include "catch.hpp"
#include "paragraph.hpp"
#include <vector>

TEST_CASE("noexcept")
{
    using namespace LegacyCode;
    std::vector<Paragraph> vec;
    //vec.reserve(5);

    vec.push_back(Paragraph("a"));
    std::cout << "\n";
    vec.push_back(Paragraph("b"));
    std::cout << "\n";
    vec.push_back(Paragraph("c"));
    std::cout << "\n";
    vec.push_back(Paragraph("d"));
    std::cout << "\n";
    vec.push_back(Paragraph("e"));
    std::cout << "\n";
    vec.emplace_back("f", 2048);

}