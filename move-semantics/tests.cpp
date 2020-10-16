#include "catch.hpp"
#include "gadget.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

string full_name(const string& first_name, const string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("refences binding")
{
    string name = "jan";

    SECTION("C++98")
    {
        string& ref_name = name; // lvalue ref can bind to lvalue

        const string& ref_full_name = full_name(name, "kowalski");

        REQUIRE(ref_full_name.size() == 12);
    }

    SECTION("C++11")
    {
        string&& rvref_full_name = full_name(name, "kowalski");

        rvref_full_name[0] = 'd';
        REQUIRE(rvref_full_name == "dan kowalski");

        // string&& rvref_name = name; // ERROR - illegal binding
    } // end of life for rvref_full_name -> temp object
}

template <typename T>
void foo(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("reference collapsing")
{
    int x = 10;
    foo<int&>(x);

    foo<int&&>(x);
}

TEST_CASE("universal reference - auto&&")
{
    auto&& universal_ref1 = 42; // int&&

    int x = 42;
    auto&& universal_ref2 = x; // int&
}

struct X
{
    int id_;
    std::vector<int> vec_;
    std::unique_ptr<Gadget> ptr_;

    X(int id, const std::vector<int>& vec, std::unique_ptr<Gadget> ptr)
        : id_ {id}
        , vec_ {vec}
        , ptr_ {std::move(ptr)}
    {
    }

    X(X&& other) noexcept
        : id_{other.id_}, vec_{std::move(other.vec_)}, ptr_{std::move(other.ptr_)}
    {
    }

    // X& operator=(X&& other)
    // {
    //     if (this != &other)
    //     {
    //         id_ = other.id_;
    //         vec_ = std::move(other.vec_);
    //         ptr_ = std::move(other.ptr_);
    //     }

    //     return *this;
    // }
};

TEST_CASE("X & move semantics")
{
    auto lvalue_ptr = std::unique_ptr<Gadget> {new Gadget(665, "ipad")};
    X x1 {42, {1, 2, 3}, std::move(lvalue_ptr)};
    x1.ptr_->use();
    REQUIRE(x1.vec_ == std::vector<int> {1, 2, 3});

    X x2 = std::move(x1);
    x2.ptr_->use();
    REQUIRE(x2.vec_ == std::vector<int> {1, 2, 3});
    REQUIRE(x1.vec_.size() == 0); // illegal
}

// namespace Cpp17
// {
//     template <typename T>
//     T make_deep_copy(const T& obj)
//     {
//         return obj;
//     }

//     template <typename T>
//     void do_some_stuff(T obj)
//     {
//         if constexpr (std::is_move_constructible_v<T>)
//         {
//             T moved_obj = std::move(obj);
//             std::cout << "Object moved\n";
//         }
//         else
//         {
//             T deep_copy = make_deep_copy(obj);
//             std::cout << "Object moved\n";
//         }
//     }
// }

struct Data
{
    std::vector<int> data_;
    std::string name_;

    //////////////////////////////////////
    // optimal efficiency - but cumbersome

    // Data(std::vector<int>&& data_param, const std::string& name_param)
    //     : data_{std::move(data_param)}, name_{name_param}
    // {
    // }

    // Data(const std::vector<int>& data_param, std::string&& name_param)
    //     : data_{data_param}, name_{std::move(name_param)}
    // {
    // }

    // simple & efficient enough - very good compromise
    Data(std::vector<int> data_param, std::string name_param)
        : data_ {std::move(data_param)}
        , name_ {std::move(name_param)}
    {
    }

    Data(const Data&) = default;
    Data& operator=(const Data&) = default;
    Data(Data&&) = default;
    Data& operator=(Data&&) = default;

    ~Data() { std::cout << "~Data()\n"; } // because of it - rule of 5!!!
};

TEST_CASE("Data - 1")
{
    string name = "d1";
    Data d1 {{1, 2, 3}, name};

    std::vector<int> vec = {1, 2, 3};
    Data alt_d1(vec, "alt_d1");

    Data d2 = std::move(d1);

    REQUIRE(d1.data_.size() == 0);
}

TEST_CASE("Data - 2")
{
    Data* d1;
    {
        string name = "d1";
        d1 = new Data {{1, 2, 3}, std::move(name)};
    }
    cout << d1->name_;

    Data d2 = std::move(*d1);

    REQUIRE(d1->data_.size() == 0);
    delete d1;
}

TEST_CASE("Data - 3")
{
    std::cout << "\n---------------\n\n";

    std::string name = "very long text very long text very long text very long text";
    std::cout << "name& " << (long long*)&name << std::endl;
    std::cout << "name.data& " << (long long*)name.c_str() << std::endl;

    Data d1 {{1, 2, 3}, std::move(name)};

    std::cout << "d1 name& " << (long long*)&d1.name_ << std::endl;
    std::cout << "d1 name.data& " << (long long*)d1.name_.c_str() << std::endl;

    std::cout << "\n---------------\n\n";
}

////////////////////////////////////////////////////////
// Matrix example

namespace legacy_code
{
    struct Matrix
    {
        std::vector<std::vector<int>> data;
    };

    Matrix* create_matrix()
    {
        Matrix* m = new Matrix {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};

        return m;
    }

    void matrix_user()
    {
        Matrix* m = create_matrix();

        for (std::vector<std::vector<int>>::const_iterator it_row = m->data.begin(); it_row != m->data.end(); ++it_row)
        {
            std::cout << "[ ";
            for (std::vector<int>::const_iterator it = it_row->begin(); it != it_row->end(); ++it)
            {
                std::cout << *it << " ";
            }
            std::cout << "]\n";
        }

    } // memory_leak
}

namespace modern_code
{
    struct Matrix
    {
        std::vector<std::vector<int>> data;

        Matrix(std::initializer_list<std::vector<int>> lst)
            : data {lst}
        {
        }

        size_t rows() const
        {
            return data.size();
        }

        auto begin()
        {
            return data.begin();
        }

        auto end()
        {
            return data.end();
        }

        auto begin() const
        {
            return data.begin();
        }

        auto end() const
        {
            return data.end();
        }
    };

    Matrix create_matrix()
    {
        return Matrix {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    }

    void matrix_user()
    {
        Matrix m = create_matrix();

        for (const auto& rows : m.data)
        {
            std::cout << "[ ";
            for (const auto& item : rows)
            {
                std::cout << item << " ";
            }
            std::cout << "]\n";
        }
    }
}

TEST_CASE("matrix")
{
    std::cout << "\n";

    legacy_code::matrix_user();

    std::cout << "\n";

    modern_code::matrix_user();
}

////////////////////////////////////////////////
// call wrapper - perfect forwarding

template <typename F, typename... Args>
void call(F&& f, Args&&... args)
{
    //arg *= 2;
    (..., (args *= 2));  // fold expression
    
    f(std::forward<Args>(args)...);
}

void foobar1(int x)
{
    std::cout << "foobar(" << x << ")\n";
}

void foobar2(int x, double d)
{
    std::cout << "foobar(" << x << ", " << d << ")\n";
}

TEST_CASE("call_wrapper")
{
    call(foobar1, 42);

    int x = 665;
    call(foobar2, x, 3.14);
}

std::vector<int> create_data()
{
    return std::vector<int>(1'000'000, -1);
}

TEST_CASE("prvalue + function")
{
    std::vector<int> vec = create_data();
}