#ifndef GADGET_HPP
#define GADGET_HPP

#include <iostream>

struct Gadget
{
    int value {};
    std::string name {};

    Gadget() = default;

    Gadget(int v)
        : value {v}
    {
        std::cout << "Gadget(" << value << ")\n";
    }

    Gadget(std::string n)
        : name {std::move(n)}
    {
        std::cout << "Gadget(" << value << ", " << name << ")\n";
    }

    Gadget(int v, std::string n)
        : value {std::move(v)}
        , name {std::move(n)}
    {
        std::cout << "Gadget(" << value << ", " << name << ")\n";
    }

    void use() const
    {
        std::cout << "Using Gadget(" << value << ")\n";
    }

    ~Gadget()
    {
        std::cout << "~Gadget(" << value << ", " << name << ")\n";
    }
};

#endif