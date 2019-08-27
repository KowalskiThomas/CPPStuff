#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>
#include <string>

class Class
{
public:
    Class(bool should_throw)
    {
        if (should_throw)
            throw std::exception();

        std::cout << "Constructed" << std::endl;
    }

    ~Class()
    {
        std::cout << "Destructed" << std::endl;
    }
};

void exec();

int main()
{
    try {
        exec();
    }
    catch(std::exception& e)
    {
        std::cout << "Exception caught" << std::endl;
    }
    return 0;
}

int f(Class, Class)
{
    return 0;
}

int f(Class* a, Class* b)
{
    delete a;
    delete b;
    return 0;
}

int f(std::unique_ptr<Class>, std::unique_ptr<Class>)
{
    return 2;
}


#include <memory>

void exec()
{
    f(std::make_unique<Class>(false), std::make_unique<Class>(true));
}