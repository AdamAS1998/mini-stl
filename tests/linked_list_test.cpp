#include <iostream>
#include <cassert>
#include "../include/ministl/containers/linked_list.hpp"

using ministl::List;

void test_push_pop(){
    List<int> l;

    assert(l.empty());

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    assert(l.size() == 3);
    assert(l.front() == 1);
    assert(l.back() == 3);

    l.pop_front();
    assert(l.front() == 2);

    l.pop_back();
    assert(l.back() == 2);

    l.pop_back();
    assert(l.empty());
}

void test_insert_erase(){
    List<int> l;

    l.push_back(1);
    l.push_back(3);
    auto it = l.begin();
    ++it;
    l.insert(it, 2);
    int expected = 1;
    for(int x : l)
        assert(x == expected++);

    it = l.begin();
    ++it;
    l.erase(it);
    assert(l.size() == 2);
    assert(l.front() == 1);
    assert(l.back() == 3);
}

void test_reverse(){
    List<int> l;
    for(int i = 1; i <= 5; i++)
        l.push_back(i);
    l.reverse();
    int expected = 5;
    for(int x : l)
        assert(x == expected--);
}

void test_unique()
{
    List<int> l;

    l.push_back(1);
    l.push_back(1);
    l.push_back(2);
    l.push_back(2);
    l.push_back(3);

    l.unique();

    int expected[] = {1,2,3};
    int i = 0;

    for(int x : l)
        assert(x == expected[i++]);
}

void test_remove(){
    List<int> l;
    for(int i = 1; i <= 5; i++)
        l.push_back(i);
    l.remove(3);
    int expected[] = {1,2,4,5};
    int i = 0;
    for(int x : l)
        assert(x == expected[i++]);
}

void test_remove_if(){
    List<int> l;
    for(int i = 1; i <= 6; i++)
        l.push_back(i);
    l.remove_if([](int x){
        return x % 2 == 0;
    });
    int expected[] = {1,3,5};
    int i = 0;
    for(int x : l)
        assert(x == expected[i++]);
}

void test_sort(){
    List<int> l;
    l.push_back(4);
    l.push_back(1);
    l.push_back(3);
    l.push_back(2);
    l.sort();
    int expected = 1;
    for(int x : l)
        assert(x == expected++);
}

void test_sort_comparator(){
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    l.sort([](int a, int b){
        return a > b;
    });
    int expected = 4;
    for(int x : l)
        assert(x == expected--);
}

void test_splice(){
    List<int> a;
    List<int> b;
    a.push_back(1);
    a.push_back(2);
    b.push_back(3);
    b.push_back(4);
    auto pos = a.end();
    a.splice(pos, b);
    int expected[] = {1,2,3,4};
    int i = 0;
    for(int x : a)
        assert(x == expected[i++]);
    assert(b.empty());
}

void test_iterator(){
    List<int> l;
    for(int i = 1; i <= 5; i++)
        l.push_back(i);

    auto it = l.begin();
    assert(*it == 1);

    ++it;
    assert(*it == 2);

    --it;
    assert(*it == 1);
}

void test_stress(){
    List<int> l;
    const int N = 100000000;
    for(int i = 0; i < N; i++)
        l.push_back(i);

    assert(l.size() == N);

    int expected = 0;
    for(int x : l)
        assert(x == expected++);

    for(int i = 0; i < N/2; i++)
        l.pop_front();

    assert(l.size() == N/2);

    for(int i = 0; i < N/2; i++)
        l.pop_back();

    assert(l.empty());
}

int main(){

    test_push_pop();
    test_insert_erase();
    test_reverse();
    test_unique();
    test_remove();
    test_remove_if();
    test_sort();
    test_sort_comparator();
    test_splice();
    test_iterator();
    test_stress();

    std::cout << "All List tests passed!\n";
}