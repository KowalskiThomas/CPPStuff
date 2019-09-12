#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>

using std::string;

void exec();

int main()
{
    exec();
    return 0;
}

template<typename Derived>
class Base
{
    Derived& as_derived() {
        return *static_cast<Derived*>(this);
    }

public:
    string do_something()
    {
        string base = "I'm base and I'm actually " + implementation();
        return base;
    }

    string implementation()
    {
        return as_derived().implementation();
    }
};

class Derived1 : public Base<Derived1>
{
public:
    string implementation()
    {
        return "a Derived One";
    }
};

class Derived2 : public Base<Derived2>
{
public:
    string implementation()
    {
        return "a Derived Two";
    }
};

template<typename Derived>
void print(Derived& d)
{
    std::cout << d.do_something();
}

void exec()
{
    Derived1 d1;
    Derived2 d2;
    std::cout << "D1: " << d1.implementation() << std::endl;

    std::cout << "D1 as Base: ";
    print(d1);
    std::cout << std::endl;;

    std::cout << "D2 as Base: ";
    print(d2);
    std::cout << std::endl;

    /* CRTP "Dynamic calls" cannot be used like that:
     * Because we can't just make a vector<Base> (without a template argument on Base)
     */
//    std::cout << "Vector thingy" << std::endl;
//    vector<Base&> elements = {d1, d2};
//    for (auto x : )
//        print(x);
}