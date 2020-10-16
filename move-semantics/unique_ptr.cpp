#include "catch.hpp"
#include "gadget.hpp"
#include <iostream>

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
    UniquePtr(UniquePtr&& other) noexcept
        : ptr_ {other.ptr_}
    {
        other.ptr_ = nullptr;
    }

    // move assignment operator
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other)
        {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr() noexcept
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

    std::cout << "\n\n";

    SECTION("move assignment")
    {
        UniquePtr<Gadget> ptr_g1 {new Gadget(1, "ipad")};

        ptr_g1 = UniquePtr<Gadget> {new Gadget(42, "smartphone")}; // implicit move of rvalue
        ptr_g1->use();
    }
} // ptr_g1 will destroy ipad

////////////////////////////////////////////////////////////
// perfect forwarding

template <typename T, typename... Args>
UniquePtr<T> make_unique_ptr(Args&&... args)
{
    puts(__PRETTY_FUNCTION__);
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

TEST_CASE("perfect forwarding")
{
    std::string name = "smartwatch";
    UniquePtr<Gadget> ptr_sw = make_unique_ptr<Gadget>(name);
    
    UniquePtr<Gadget> ptr_g = make_unique_ptr<Gadget>(1, "tablet");
    ptr_g->use();

    UniquePtr<Gadget> ptr_g2 = make_unique_ptr<Gadget>();
    ptr_g2->use();
}