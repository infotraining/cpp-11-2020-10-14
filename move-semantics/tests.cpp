#include "catch.hpp"
#include <iostream>
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

struct Gadget
{
    int value {};
    std::string name {};

    Gadget() = default;

    Gadget(int v)
        : value {v}
    {
        cout << "Gadget(" << value << ")\n";
    }

    Gadget(std::string n) : name{std::move(n)}
    {        
        cout << "Gadget(" << value << ", " << name << ")\n";
    }

    Gadget(int v, std::string n)
        : value {std::move(v)}
        , name {std::move(n)}
    {
        cout << "Gadget(" << value << ", " << name << ")\n";
    }

    void use() const
    {
        cout << "Using Gadget(" << value << ")\n";
    }

    ~Gadget()
    {
        cout << "~Gadget(" << value << ", " << name << ")\n";
    }
};

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    explicit UniquePtr(T* ptr)
        : ptr_ {ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& other)
        : ptr_ {other.ptr_}
    {
        other.ptr_ = nullptr;
    }

    // move assignment operator
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (this != &other)
        {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return *ptr_;
    }

    T* get() const
    {
        return ptr_;
    }
};

TEST_CASE("unique pointer")
{
    SECTION("move constructor")
    {
        UniquePtr<Gadget> ptr_g1 {new Gadget(1, "ipad")};
        UniquePtr<Gadget> ptr_g2 = std::move(ptr_g1); // explicit move on lvalue
        ptr_g2->use();
    }

    cout << "\n\n";

    SECTION("move assignment")
    {
        UniquePtr<Gadget> ptr_g1 {new Gadget(1, "ipad")};

        ptr_g1 = UniquePtr<Gadget> {new Gadget(42, "smartphone")}; // implicit move of rvalue
        ptr_g1->use();
    }
} // ptr_g1 will destroy ipad

template <typename T>
void foo(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("reference collapsing")
{
    int x = 10;
    foo<int&>(x);
}

template <typename T, typename Arg>
UniquePtr<T> make_unique_ptr(Arg&& arg)
{
    puts(__PRETTY_FUNCTION__);
    return UniquePtr<T>(new T(std::forward<Arg>(arg)));
}

TEST_CASE("perfect forwarding")
{
    string name = "smartwatch";
    UniquePtr<Gadget> ptr_sw = make_unique_ptr<Gadget>(name);
    
    UniquePtr<Gadget> ptr_g = make_unique_ptr<Gadget>("tablet");
    ptr_g->use();
}

TEST_CASE("auto + &&")
{
    auto&& universal_ref1 = 42; // int&&

    int x = 42;
    auto&& universal_ref2 = x; // int&
}


struct X
{
    int id_;
    std::vector<int> vec_;
    UniquePtr<Gadget> ptr_;

    X(int id, const std::vector<int>& vec, UniquePtr<Gadget> ptr)
        : id_ {id}
        , vec_ {vec}
        , ptr_ {std::move(ptr)}
    {
    }

    // X(X&& other)
    //     : id_{other.id_}, vec_{std::move(other.vec_)}, ptr_{std::move(other.ptr_)}
    // {
    // }

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
    auto lvalue_ptr = UniquePtr<Gadget> {new Gadget(665, "ipad")};
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

    // Data(std::vector<int>&& data_param, const std::string& name_param)
    //     : data_{std::move(data_param)}, name_{name_param}
    // {
    // }

    // Data(const std::vector<int>& data_param, std::string&& name_param)
    //     : data_{data_param}, name_{std::move(name_param)}
    // {
    // }

    Data(std::vector<int> data_param, std::string name_param)
        : data_{std::move(data_param)}, name_{std::move(name_param)}
    {
    }

    Data(const Data&) = default;
    Data& operator=(const Data&) = default;
    Data(Data&&) = default;
    Data& operator=(Data&&) = default;

    ~Data() { std::cout << "~Data()\n"; }
};

TEST_CASE("Data")
{
    string name = "d1";
    Data d1{{1, 2, 3}, name};

    std::vector<int> vec = {1, 2, 3};
    Data alt_d1(vec, "alt_d1");

    Data d2 = std::move(d1);

    REQUIRE(d1.data_.size() == 0);
}

TEST_CASE("Data2")
{
    Data* d1;
    {
        string name = "d1";
        d1 = new Data{{1, 2, 3}, std::move(name)};
    }
    cout << d1->name_;

    Data d2 = std::move(*d1);

    REQUIRE(d1->data_.size() == 0);
    delete d1;
}

TEST_CASE("Data3")
{
    std::cout << "\n---------------\n\n";

    std::string name = "dgjadshgjlsdfjksfdhgjklsdfh gjh gjlkhdfkljh1";
    std::cout << "name& " << (long long*) &name << std::endl;
    std::cout << "name.data& " << (long long*) name.c_str() << std::endl;
    
    Data d1{{1, 2, 3}, std::move(name)};

    std::cout << "d1 name& " << (long long*) &d1.name_ << std::endl;
    std::cout << "d1 name.data& " << (long long*) d1.name_.c_str() << std::endl;

    std::cout << "\n---------------\n\n";
}

namespace legacy_code
{
    struct Matrix
    {
        std::vector<int> rows;
    };

    Matrix* create_matrix()
    {
        return new Matrix {{1, 2, 3}};
    }

    void matrix_user()
    {
        Matrix* m = create_matrix();

        m->rows.size();

    } // memory_leak
}

namespace modern_code
{
    struct Matrix
    {
        std::vector<int> rows;
    };

    Matrix create_matrix()
    {
        return Matrix {{1, 2, 3}};
    }

    void matrix_user()
    {
        Matrix m = create_matrix();
    }
}

template <typename F, typename Arg>
void call(F&& f, Arg&& arg)
{
    arg *= 2;
    f(std::forward<Arg>(arg));
}

void foobar(int x)
{
    std::cout << "foobar(" << x << ")\n";
}

TEST_CASE("call_wrapper")
{
    call(foobar, 42);

    int x = 665;
    call(foobar, x);    
}