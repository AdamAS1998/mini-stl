#include <cassert>
#include "../include/ministl/containers/vector.hpp"


int main()
{
    ministl::Vector<int> v;

    // push_back test
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    // pop_back test
    v.pop_back();

    assert(v.size() == 2);
    assert(v[1] == 2);

    // reserve test
    v.reserve(100);

    assert(v.capacity() >= 100);

    // iterator test
    int sum = 0;

    for (auto x : v)
    {
        sum += x;
    }

    assert(sum == 3); // 1 + 2

    return 0;
}
