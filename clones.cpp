#include <ostream>
#include <cassert>

template<class D, class B>
struct derivesFrom
{
    class No{};
    class Yes { No no[2]; };

    static Yes test(B*);
    static No test(...);

    static const constexpr bool result = sizeof(test(static_cast<D*>(0))) == sizeof(Yes);
};

template<class T>
struct hasClone
{
    class No {};
    class Yes { No no[2]; };

    Yes test() {
    }

    No test(...);

    static const constexpr bool value = sizeof(decltype(std::declval<hasClone*>()->test())) == sizeof(No) ? false : true;

    static bool thomas()
    {
/*
        T* (T::*test)() = &T::clone;
        test;
*/
        return true;
    }

    ~hasClone()
    {
        thomas();
    }
};

class clonable : hasClone<clonable>
{
public:
    clonable* clone() {
        return nullptr;
    }
};

class noclone // : hasClone<noclone>
{

};

class base
{
};

class derives : public base
{

};

class noderives
{

};

int main()
{
    clonable c;
    noclone nc;

    // hasClone<noclone>::value;

    auto ok = derivesFrom<derives, base>::result;

    auto ok2 = !derivesFrom<noderives, base>::result;

    assert(ok);
    assert(ok2);
}