#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>

void exec();

int main()
{
    exec();
    return 0;
}


class FuncPtr_;
// A FuncPtr is a function that returns a FuncPtr_ and takes no argument
typedef FuncPtr_ (*FuncPtr)();

class FuncPtr_
{
public:
    FuncPtr_(FuncPtr p)
        : p(p)
    {
    }

    operator FuncPtr()
    {
        return p;
    }

    FuncPtr p;
};

FuncPtr_ f()
{
    std::cout << "F called" << std::endl;
    return f;
}

void exec()
{
    auto g = f();
    g();
}