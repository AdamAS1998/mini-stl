#include <iostream>
#include "ministl/containers/vector.hpp"

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
}
