#include <cassert>
#include <unordered_map>
#include <string>
#include <random>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "../include/ministl/containers/hashmap.hpp"

using namespace ministl;

/** AI GENERATED TESTS **/

// ============================================================
// Collision hash
//
// Every key maps to the same hash value.
// This intentionally forces collisions.
// ============================================================

struct ConstantHash {

    std::size_t operator()(int) const {
        return 0;
    }
};


// ============================================================
// Custom mapped type
// ============================================================

struct Person {

    std::string name;
    int age;

    Person()
            : name(), age(0) {}

    Person(std::string n, int a)
            : name(std::move(n)),
              age(a) {}
};


// ============================================================
// Default construction
// ============================================================

void test_default_construction() {

    HashMap<int, int> map;

    assert(map.empty());
    assert(map.size() == 0);

    assert(map.bucket_count() > 0);
}


// ============================================================
// Basic insert
// ============================================================

void test_insert() {

    HashMap<int, std::string> map;

    assert(map.insert(1, "one"));
    assert(map.insert(2, "two"));
    assert(map.insert(3, "three"));

    assert(map.size() == 3);

    assert(map.at(1) == "one");
    assert(map.at(2) == "two");
    assert(map.at(3) == "three");
}


// ============================================================
// Duplicate insert
// ============================================================

void test_duplicate_insert() {

    HashMap<int, std::string> map;

    assert(map.insert(1, "one"));

    assert(!map.insert(1, "different"));

    assert(map.size() == 1);

    assert(map.at(1) == "one");
}


// ============================================================
// operator[]
// ============================================================

void test_subscript() {

    HashMap<std::string, int> map;

    map["Adam"] = 10;
    map["John"] = 20;

    assert(map.size() == 2);

    assert(map["Adam"] == 10);
    assert(map["John"] == 20);


    // Existing key should not create another element.
    map["Adam"] = 100;

    assert(map.size() == 2);
    assert(map["Adam"] == 100);
}


// ============================================================
// operator[] default insertion
// ============================================================

void test_subscript_default_value() {

    HashMap<std::string, int> map;

    assert(!map.contains("Adam"));

    int value = map["Adam"];

    assert(value == 0);

    assert(map.contains("Adam"));
    assert(map.size() == 1);
}


// ============================================================
// at()
// ============================================================

void test_at() {

    HashMap<int, std::string> map;

    map.insert(10, "ten");

    assert(map.at(10) == "ten");


    bool threw = false;

    try {

        map.at(999);
    }
    catch (const std::out_of_range&) {

        threw = true;
    }

    assert(threw);
}


// ============================================================
// const at()
// ============================================================

void test_const_at() {

    HashMap<int, std::string> map;

    map.insert(1, "one");
    map.insert(2, "two");

    const HashMap<int, std::string>& const_map = map;

    assert(const_map.at(1) == "one");
    assert(const_map.at(2) == "two");
}


// ============================================================
// contains
// ============================================================

void test_contains() {

    HashMap<int, int> map;

    map.insert(10, 100);
    map.insert(20, 200);

    assert(map.contains(10));
    assert(map.contains(20));

    assert(!map.contains(30));
}


// ============================================================
// find
// ============================================================

void test_find() {

    HashMap<int, std::string> map;

    map.insert(10, "ten");
    map.insert(20, "twenty");

    auto it = map.find(10);

    assert(it != map.end());

    assert(it->first == 10);
    assert(it->second == "ten");


    auto missing = map.find(999);

    assert(missing == map.end());
}


// ============================================================
// Modify through iterator
// ============================================================

void test_iterator_modify_value() {

    HashMap<int, int> map;

    map.insert(1, 10);

    auto it = map.find(1);

    assert(it != map.end());

    it->second = 100;

    assert(map.at(1) == 100);
}


// ============================================================
// const find
// ============================================================

void test_const_find() {

    HashMap<int, std::string> map;

    map.insert(1, "one");

    const HashMap<int, std::string>& const_map = map;

    auto it = const_map.find(1);

    assert(it != const_map.end());

    assert(it->first == 1);
    assert(it->second == "one");
}


// ============================================================
// Erase
// ============================================================

void test_erase() {

    HashMap<int, int> map;

    map.insert(1, 10);
    map.insert(2, 20);
    map.insert(3, 30);

    assert(map.size() == 3);


    assert(map.erase(2));

    assert(map.size() == 2);

    assert(!map.contains(2));

    assert(map.contains(1));
    assert(map.contains(3));


    assert(!map.erase(999));

    assert(map.size() == 2);
}


// ============================================================
// Erase everything
// ============================================================

void test_erase_all() {

    HashMap<int, int> map;

    constexpr int N = 1000;

    for (int i = 0; i < N; ++i)
        map.insert(i, i * 10);


    for (int i = 0; i < N; ++i) {

        assert(map.erase(i));
    }


    assert(map.empty());
    assert(map.size() == 0);
}


// ============================================================
// Clear
// ============================================================

void test_clear() {

    HashMap<int, int> map;

    for (int i = 0; i < 1000; ++i)
        map.insert(i, i);


    assert(map.size() == 1000);


    map.clear();


    assert(map.empty());
    assert(map.size() == 0);


    // Map should still be reusable.
    map.insert(100, 200);

    assert(map.size() == 1);
    assert(map.at(100) == 200);
}


// ============================================================
// Forced collisions
// ============================================================

void test_collisions() {

    HashMap<
            int,
            std::string,
            ConstantHash
    > map;


    for (int i = 0; i < 100; ++i) {

        map.insert(
                i,
                "value" + std::to_string(i)
        );
    }


    assert(map.size() == 100);


    for (int i = 0; i < 100; ++i) {

        assert(map.contains(i));

        assert(
                map.at(i) ==
                "value" + std::to_string(i)
        );
    }
}


// ============================================================
// Collision erase
// ============================================================

void test_collision_erase() {

    HashMap<
            int,
            int,
            ConstantHash
    > map;


    for (int i = 0; i < 100; ++i)
        map.insert(i, i * 10);


    for (int i = 0; i < 100; i += 2)
        assert(map.erase(i));


    for (int i = 0; i < 100; ++i) {

        if (i % 2 == 0) {

            assert(!map.contains(i));
        }
        else {

            assert(map.contains(i));

            assert(map.at(i) == i * 10);
        }
    }
}


// ============================================================
// Automatic rehash
// ============================================================

void test_automatic_rehash() {

    HashMap<int, int> map;

    std::size_t initial_buckets =
            map.bucket_count();


    for (int i = 0; i < 1000; ++i)
        map.insert(i, i);


    assert(map.bucket_count() > initial_buckets);

    assert(map.size() == 1000);


    // Every element must survive rehashing.
    for (int i = 0; i < 1000; ++i) {

        assert(map.contains(i));
        assert(map.at(i) == i);
    }
}


// ============================================================
// Manual rehash
// ============================================================

void test_manual_rehash() {

    HashMap<int, int> map;

    for (int i = 0; i < 100; ++i)
        map.insert(i, i * 2);


    map.rehash(500);


    assert(map.bucket_count() >= 500);

    assert(map.size() == 100);


    for (int i = 0; i < 100; ++i)
        assert(map.at(i) == i * 2);
}


// ============================================================
// Reserve
// ============================================================

void test_reserve() {

    HashMap<int, int> map;

    map.reserve(1000);

    std::size_t buckets =
            map.bucket_count();


    assert(buckets >= 1000);


    for (int i = 0; i < 1000; ++i)
        map.insert(i, i);


    /*
        max_load_factor defaults to 1.0.

        Therefore the reserved bucket count should be enough
        for 1000 elements without requiring another growth.
    */
    assert(map.bucket_count() == buckets);


    for (int i = 0; i < 1000; ++i)
        assert(map.at(i) == i);
}


// ============================================================
// Load factor
// ============================================================

void test_load_factor() {

    HashMap<int, int> map(10);

    assert(map.load_factor() == 0.0f);


    for (int i = 0; i < 5; ++i)
        map.insert(i, i);


    float expected =
            static_cast<float>(map.size()) /
            static_cast<float>(map.bucket_count());


    assert(map.load_factor() == expected);
}


// ============================================================
// max_load_factor
// ============================================================

void test_max_load_factor() {

    HashMap<int, int> map;

    map.max_load_factor(0.5f);

    assert(map.max_load_factor() == 0.5f);


    for (int i = 0; i < 1000; ++i)
        map.insert(i, i);


    assert(
            map.load_factor()
            <= map.max_load_factor()
    );


    for (int i = 0; i < 1000; ++i)
        assert(map.at(i) == i);
}


// ============================================================
// Emplace
// ============================================================

void test_emplace() {

    HashMap<int, Person> map;

    assert(
            map.emplace(
                    1,
                    "Adam",
                    28
            )
    );


    assert(
            map.emplace(
                    2,
                    "John",
                    30
            )
    );


    assert(map.size() == 2);

    assert(map.at(1).name == "Adam");
    assert(map.at(1).age == 28);

    assert(map.at(2).name == "John");
    assert(map.at(2).age == 30);


    // Duplicate key must not replace the old value.
    assert(
            !map.emplace(
                    1,
                    "Different",
                    100
            )
    );


    assert(map.at(1).name == "Adam");
}


// ============================================================
// Iteration
// ============================================================

void test_iteration() {

    HashMap<int, int> map;

    constexpr int N = 1000;


    long long expected_sum = 0;


    for (int i = 0; i < N; ++i) {

        map.insert(i, i);

        expected_sum += i;
    }


    long long actual_sum = 0;

    std::size_t count = 0;


    for (auto it = map.begin();
         it != map.end();
         ++it) {

        actual_sum += it->second;

        ++count;
    }


    assert(count == map.size());

    assert(actual_sum == expected_sum);
}


// ============================================================
// Range-based for
// ============================================================

void test_range_based_for() {

    HashMap<int, int> map;

    for (int i = 0; i < 100; ++i)
        map.insert(i, i);


    int count = 0;


    for (auto& entry : map) {

        entry.second *= 2;

        ++count;
    }


    assert(count == 100);


    for (int i = 0; i < 100; ++i)
        assert(map.at(i) == i * 2);
}


// ============================================================
// Const iteration
// ============================================================

void test_const_iteration() {

    HashMap<int, int> map;

    for (int i = 0; i < 100; ++i)
        map.insert(i, i);


    const HashMap<int, int>& const_map = map;


    std::size_t count = 0;


    for (auto it = const_map.begin();
         it != const_map.end();
         ++it) {

        assert(
                it->second ==
                it->first
        );

        ++count;
    }


    assert(count == 100);
}


// ============================================================
// Iterator -> const_iterator
// ============================================================

void test_iterator_to_const_iterator() {

    HashMap<int, int> map;

    map.insert(10, 100);


    HashMap<int, int>::iterator it =
            map.find(10);


    HashMap<int, int>::const_iterator cit =
            it;


    assert(cit->first == 10);
    assert(cit->second == 100);
}


// ============================================================
// Copy constructor
// ============================================================

void test_copy_constructor() {

    HashMap<int, std::string> original;


    for (int i = 0; i < 100; ++i) {

        original.insert(
                i,
                std::to_string(i)
        );
    }


    HashMap<int, std::string> copy(original);


    assert(copy.size() == original.size());


    for (int i = 0; i < 100; ++i) {

        assert(
                copy.at(i) ==
                original.at(i)
        );
    }


    // Verify independence.
    copy[0] = "changed";


    assert(copy.at(0) == "changed");

    assert(original.at(0) == "0");
}


// ============================================================
// Copy assignment
// ============================================================

void test_copy_assignment() {

    HashMap<int, int> a;
    HashMap<int, int> b;


    for (int i = 0; i < 100; ++i)
        a.insert(i, i * 10);


    b.insert(1000, 1000);


    b = a;


    assert(b.size() == a.size());


    for (int i = 0; i < 100; ++i)
        assert(b.at(i) == i * 10);


    b.erase(0);


    assert(!b.contains(0));

    assert(a.contains(0));
}


// ============================================================
// Self copy assignment
// ============================================================

void test_self_copy_assignment() {

    HashMap<int, int> map;


    for (int i = 0; i < 100; ++i)
        map.insert(i, i);


    map = map;


    assert(map.size() == 100);


    for (int i = 0; i < 100; ++i)
        assert(map.at(i) == i);
}


// ============================================================
// Move constructor
// ============================================================

void test_move_constructor() {

    HashMap<int, int> original;


    for (int i = 0; i < 100; ++i)
        original.insert(i, i);


    HashMap<int, int> moved(
            std::move(original)
    );


    assert(moved.size() == 100);


    for (int i = 0; i < 100; ++i)
        assert(moved.at(i) == i);


    assert(original.empty());
}


// ============================================================
// Move assignment
// ============================================================

void test_move_assignment() {

    HashMap<int, int> a;
    HashMap<int, int> b;


    for (int i = 0; i < 100; ++i)
        a.insert(i, i);


    b.insert(1000, 1000);


    b = std::move(a);


    assert(b.size() == 100);


    for (int i = 0; i < 100; ++i)
        assert(b.at(i) == i);


    assert(a.empty());
}


// ============================================================
// Swap
// ============================================================

void test_swap() {

    HashMap<int, std::string> a;
    HashMap<int, std::string> b;


    a.insert(1, "one");
    a.insert(2, "two");


    b.insert(10, "ten");
    b.insert(20, "twenty");
    b.insert(30, "thirty");


    a.swap(b);


    assert(a.size() == 3);

    assert(a.at(10) == "ten");
    assert(a.at(20) == "twenty");
    assert(a.at(30) == "thirty");


    assert(b.size() == 2);

    assert(b.at(1) == "one");
    assert(b.at(2) == "two");
}


// ============================================================
// Strings as keys
// ============================================================

void test_string_keys() {

    HashMap<std::string, int> map;


    map.insert("Adam", 10);
    map.insert("John", 20);
    map.insert("Alice", 30);


    assert(map.at("Adam") == 10);
    assert(map.at("John") == 20);
    assert(map.at("Alice") == 30);


    assert(map.contains("Adam"));

    assert(!map.contains("Bob"));
}


// ============================================================
// Compare deterministic behavior with std::unordered_map
// ============================================================

void test_vs_std_unordered_map() {

    HashMap<int, int> mine;

    std::unordered_map<int, int> standard;


    constexpr int N = 10000;


    for (int i = 0; i < N; ++i) {

        int key =
                (i * 37) % 20011;

        int value =
                i * 10;


        mine[key] = value;

        standard[key] = value;
    }


    assert(
            mine.size() ==
            standard.size()
    );


    for (const auto& entry : standard) {

        assert(
                mine.contains(entry.first)
        );

        assert(
                mine.at(entry.first) ==
                entry.second
        );
    }
}


// ============================================================
// Randomized stress against std::unordered_map
// ============================================================

void test_random_stress() {

    HashMap<int, int> mine;

    std::unordered_map<int, int> standard;


    std::mt19937 rng(42);


    std::uniform_int_distribution<int>
            operation(0, 4);

    std::uniform_int_distribution<int>
            key_dist(-5000, 5000);

    std::uniform_int_distribution<int>
            value_dist(-100000, 100000);


    constexpr int OPERATIONS = 100000;


    for (int i = 0; i < OPERATIONS; ++i) {

        int op = operation(rng);

        int key = key_dist(rng);


        if (op == 0) {

            int value =
                    value_dist(rng);


            mine[key] = value;

            standard[key] = value;
        }

        else if (op == 1) {

            bool mine_removed =
                    mine.erase(key);


            bool std_removed =
                    standard.erase(key) != 0;


            assert(
                    mine_removed ==
                    std_removed
            );
        }

        else if (op == 2) {

            assert(
                    mine.contains(key) ==
                    (standard.find(key)
                     != standard.end())
            );
        }

        else if (op == 3) {

            int value =
                    value_dist(rng);


            bool mine_inserted =
                    mine.insert(key, value);


            bool std_inserted =
                    standard.insert(
                            {key, value}
                    ).second;


            assert(
                    mine_inserted ==
                    std_inserted
            );
        }

        else {

            auto mine_it =
                    mine.find(key);


            auto std_it =
                    standard.find(key);


            assert(
                    (mine_it == mine.end()) ==
                    (std_it == standard.end())
            );


            if (std_it != standard.end()) {

                assert(
                        mine_it->second ==
                        std_it->second
                );
            }
        }


        assert(
                mine.size() ==
                standard.size()
        );
    }


    // Final complete verification.
    for (const auto& entry : standard) {

        assert(
                mine.contains(entry.first)
        );

        assert(
                mine.at(entry.first) ==
                entry.second
        );
    }
}


// ============================================================
// Large stress
// ============================================================

void test_large_stress() {

    constexpr int N = 100000;


    HashMap<int, int> map;


    for (int i = 0; i < N; ++i)
        map.insert(i, i * 2);


    assert(map.size() == N);


    for (int i = 0; i < N; ++i) {

        assert(map.contains(i));

        assert(map.at(i) == i * 2);
    }


    for (int i = 0; i < N; i += 2)
        assert(map.erase(i));


    assert(
            map.size() ==
            static_cast<std::size_t>(N / 2)
    );


    for (int i = 0; i < N; ++i) {

        if (i % 2 == 0)
            assert(!map.contains(i));

        else
            assert(map.contains(i));
    }
}


// ============================================================
// main
// ============================================================

int main() {

    test_default_construction();

    test_insert();
    test_duplicate_insert();

    test_subscript();
    test_subscript_default_value();

    test_at();
    test_const_at();

    test_contains();

    test_find();
    test_iterator_modify_value();
    test_const_find();

    test_erase();
    test_erase_all();

    test_clear();

    test_collisions();
    test_collision_erase();

    test_automatic_rehash();
    test_manual_rehash();
    test_reserve();

    test_load_factor();
    test_max_load_factor();

    test_emplace();

    test_iteration();
    test_range_based_for();
    test_const_iteration();
    test_iterator_to_const_iterator();

    test_copy_constructor();
    test_copy_assignment();
    test_self_copy_assignment();

    test_move_constructor();
    test_move_assignment();

    test_swap();

    test_string_keys();

    test_vs_std_unordered_map();

    test_random_stress();
    test_large_stress();


    std::cout
            << "All HashMap tests passed!\n";


    return 0;
}