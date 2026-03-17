#include <iostream>
#include "ministl/containers/deque.hpp"

int main() {
    using namespace ministl;
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_front(0);

    assert(d[0] == 0);
    assert(d[1] == 1);
    assert(d[2] == 2);

    d.pop_front();
    d.pop_back();

    assert(d.size() == 1);
    assert(d[0] == 1);
}

/*
    ministl::List<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::cout << "List: ";

    for(auto it = list.begin(); it != list.end(); ++it)
        std::cout << *it << " ";

    list.push_front(0);

    std::cout << "List: ";

    for(auto it = list.begin(); it != list.end(); ++it)
        std::cout << *it << " ";

    list.pop_back();

    std::cout << "List: ";

    for(auto it = list.begin(); it != list.end(); ++it)
        std::cout << *it << " ";


    auto it = list.begin();
    ++it;

    list.insert(it, 99);

    std::cout << "List: ";

    for(auto it2 = list.begin(); it2 != list.end(); ++it2)
        std::cout << *it2 << " ";

    auto it3 = list.begin();
    ++it3;

    list.erase(it3);

    std::cout << "List: ";

    for(auto it4 = list.begin(); it4 != list.end(); ++it4)
        std::cout << *it4 << " ";


}
*/


/*
#include <iostream>
#include "ministl/containers/vector.hpp"
#include "ministl/containers/linked_list.hpp"

int main()
{
    ministl::Vector<int> v;

    std::cout << "Pushing values...\n";

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    std::cout << "Vector contents:\n";

    for (auto x : v)
    {
        std::cout << x << " ";
    }

    std::cout << "\n";

    std::cout << "size: " << v.size() << "\n";
    std::cout << "capacity: " << v.capacity() << "\n";

    v.pop_back();

    std::cout << "After pop_back:\n";

    for (auto x : v)
    {
        std::cout << x << " ";
    }

    std::cout << "\n";

    return 0;
}*/
