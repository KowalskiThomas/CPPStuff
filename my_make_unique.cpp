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

namespace my {
    template<typename Pointee>
    std::unique_ptr<Pointee> mku(Pointee *p) {
        return std::unique_ptr<Pointee>(p);
    }

    template<typename Pointee, typename ... Args>
    std::unique_ptr<Pointee> mku(Args... args) {
        return std::unique_ptr<Pointee>(new Pointee(args...));
    }
}

class klass
{
public:
    klass(int a, bool b, double c)
    {

    }

    ~klass() = default;
};

void exec()
{
    auto method1 = my::mku(new klass(1, false, 3.14));
    auto method2 = my::mku<klass>(1, false, 3.14);
    auto fail = my::mku<klass>("incompatible", false);
    assert(typeid(method1).hash_code() == typeid(method2).hash_code());
}