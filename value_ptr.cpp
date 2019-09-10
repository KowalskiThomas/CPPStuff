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

template <typename T>
class ValuePtr
{
    T* ptr = nullptr;
public:
    ValuePtr() {}

    ValuePtr(T* ptr) : ptr(ptr) {}

    // Copy
    ValuePtr(const ValuePtr& o)
    {
        delete ptr;
        ptr = o.ptr ? new T(*o.ptr) : nullptr;
    }

    ValuePtr& operator=(const ValuePtr& o)
    {
        ValuePtr temp(o);
        std::swap(ptr, temp.ptr);
        return *this;
    }

    T& operator*() const
    {
        return *ptr;
    }

    ~ValuePtr()
    {
        delete ptr;
    }
};

class Member
{
    std::string tag;
public:
    Member(std::string tag) : tag(tag) {}

    ~Member()
    {
        std::cout << "Deleting member '" << tag << "'" << std::endl;
    }

    std::string getTag() const
    {
        return tag;
    }
};

class Owner
{
    std::string tag;
    ValuePtr<Member> member;

public:
    Owner(std::string tag) : tag(tag), member(new Member("Child of " + tag))
    {
    }

    void print() const
    {
        std::cout << tag << " : " << (*member).getTag() << std::endl;
    }

    void setMember(Member* m)
    {
        member = m;
    }

};

void exec()
{
    auto a = Owner("A");
    auto b = Owner("B");
    Owner c("C");
    a = Owner(b);
    a = c;
    a.print();
    Member* m = new Member("Custom member");
    a.setMember(m);
    std::cout << "Finished" << std::endl;
}