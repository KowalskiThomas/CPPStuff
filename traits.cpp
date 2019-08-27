#include <iostream>
#include <ostream>

#include <map>
#include <string>

using mapString = std::map<int, std::string>;

template<typename T>
struct CollectionTraits {
    void insert(typename T::key_type, typename T::mapped_type);
};

template<>
struct CollectionTraits<mapString>
{
/*    static void insert(mapString* p, mapString::key_type k, mapString ::mapped_type v) {
        p->insert(std::make_pair(k, v));
    }*/

    static constexpr mapString::mapped_type& (mapString::*insert) (const mapString::key_type &) = &mapString::at;
};

template<typename Container>
class Collection {
    using traits = CollectionTraits<Container>;

    Container* collection;

public:
    void insert(typename Container::key_type k, typename Container::mapped_type v)
    {
        (*collection.(*traits::insert))(k, v);
        // std::invoke(traits::insert, collection, k, v);
        // traits::insert(collection, k, v);
    }

    const Container* const wrapped() const
    {
        return collection;
    }

    Collection()
    {
        collection = new Container();
    }
};

int main()
{
    Collection<mapString> collection;
    collection.insert(1, "un");
    auto wrapped = collection.wrapped();
    for(auto val : *wrapped)
    {
        std::cout << val.first << ' ' << val.second << std::endl;
    }
    return 0;
}