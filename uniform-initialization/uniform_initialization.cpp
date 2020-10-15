#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

struct Vector2D
{
    int x {1};
    int y {0};

    Vector2D(int x, int y)
        : x(x)
        , y {y}
    {
    }
};

TEST_CASE("uniform initialization")
{
    SECTION("C++98")
    {
        int x1 = 1;
        int x2(10);
        int x3(); // function declaration

        int tab[3] = {1, 2, 3};
        Vector2D vec(1, 2);

        std::vector<int> data;
        data.push_back(1);
        data.push_back(2);
        data.push_back(3);
    }

    SECTION("C++11")
    {
        int x1 = 1;
        int x2 {10};
        int x3 {};
        int* ptr1 {}; // int* ptr1 = nullptr;

        int tab[3] = {1, 2, 3};
        Vector2D vec {1, 2};

        const std::vector<int> data = {1, 2, 3};
    }
}

struct Aggregate
{
    int a, b;
    double coord[3];
    std::string name;

    void print() const
    {
        std::cout << "Aggregate{" << a << ", " << b << ", { ";
        for (const auto& c : coord)
            std::cout << c << " ";
        std::cout << "}, " << name << "}\n";
    }
};

struct GreaterThan
{
    int value;

    GreaterThan() = delete; // user-declared

    bool operator()(int v) const
    {
        return v > value;
    }
};

TEST_CASE("Aggregate initialization")
{
    Aggregate agg1 {1, 2, {3.14, 2.71, 1.99}, "agg1"};
    agg1.print();
    Aggregate agg2 {};
    agg2.print();

    //GreaterThan gt1;
    GreaterThan gt_than_zero {};
    REQUIRE(gt_than_zero(20));
}

uint64_t generate_id()
{
    static uint64_t id {};
    return ++id;
}

struct Data
{
    const uint64_t id {generate_id()};
    std::vector<int> data {1, 2, 3};

    Data() = default;

    Data(const std::vector<int>& v)
        : data {v}
    {
    }
};

TEST_CASE("Data")
{
    Data d;
    REQUIRE(d.id == 1);
    REQUIRE(d.data == vector<int> {1, 2, 3});

    Data other({6, 6, 5});
    REQUIRE(other.id == 2);
    REQUIRE(other.data == vector<int> {6, 6, 5});

    uint64_t id {generate_id()};
}

class DataSet
{
    int* array_;
    size_t size_;

public:
    DataSet(size_t size)
        : array_ {new int[size]}
        , size_ {size}
    {
        std::fill(array_, array_ + size_, 0);
    }

    DataSet(std::initializer_list<int> lst)
        : array_ {new int[lst.size()]}
        , size_ {lst.size()}
    {
        std::copy(lst.begin(), lst.end(), array_);
    }

    DataSet(const DataSet&) = delete;
    DataSet& operator=(const DataSet&) = delete;

    ~DataSet()
    {
        delete[] array_;
    }

    size_t size() const
    {
        return size_;
    }

    int& operator[](size_t index)
    {
        return array_[index];
    }

    const int& operator[](size_t index) const
    {
        return array_[index];
    }
};

TEST_CASE("std::initializer_list")
{
    std::vector<int> vec1 {1, 2, 3}; // std::vector<int>(std::initializer_list<int>)
    std::vector<int> vec2 = {1, 2, 3}; // std::vector<int>(std::initializer_list<int>)

    DataSet ds1 = {1, 2, 3};
    REQUIRE(ds1.size() == 3);
    REQUIRE(ds1[1] == 2);

    auto lst = {generate_id(), generate_id(), generate_id()};
}

TEST_CASE("overloading constructors with std::initializer_list")
{
    DataSet ds1(10); // [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

    DataSet ds2 {10}; // [10]

    std::vector<int> vec1(3, 1); // [1, 1, 1]
    std::vector<int> vec2 {3, 1}; // [3, 1]

    std::vector<std::string> vec3 {3, "text"}; // ["text", "text", "text"]
}