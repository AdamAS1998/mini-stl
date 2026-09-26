/*
    MiniSTL - HashMap
    -----------------
    A simplified implementation of std::unordered_map for
    educational purposes.

    HashMap stores key-value pairs using a hash table with
    separate chaining for collision resolution.

    Each key is converted into a bucket index using: bucket = hash(key) % bucket_count

    If multiple keys map to the same bucket, they are stored
    in a linked list inside that bucket.

    Features:
        - key-value storage
        - separate chaining
        - automatic rehashing
        - configurable hash function
        - configurable key equality
        - Rule of Five
        - insert / emplace
        - operator[]
        - at
        - find
        - contains
        - erase
        - reserve / rehash
        - load factor management

    Average complexity:
        - insert   : O(1)
        - find     : O(1)
        - contains : O(1)
        - erase    : O(1)
        - []       : O(1)

    Worst-case complexity:
        - insert / find / erase : O(n)

    Rehash:
        - O(n)

    Space:
        - O(n + bucket_count)

    Author: Adam Abu Saleh
*/

#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace ministl {

    template<
            typename Key,
            typename T,
            typename Hash = std::hash<Key>,
            typename KeyEqual = std::equal_to<Key>
    >
    class HashMap {

    public:

        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;
        using hasher = Hash;
        using key_equal = KeyEqual;

    private:

    /*
        Node

        Each node stores one key-value pair and a pointer
        to the next node in the same bucket.
    */
        struct Node {

            value_type data;
            Node* next;

            template<typename K, typename V>
            Node(K&& key, V&& value, Node* next_node = nullptr)
                    : data(
                    std::forward<K>(key),
                    std::forward<V>(value)
            ),
                      next(next_node) {}
        };


        Node** buckets_;
        size_t bucket_count_;
        size_t size_;

        float max_load_factor_;

        hasher hash_;
        key_equal equal_;


        static constexpr size_t DEFAULT_BUCKET_COUNT = 8;


    /*
        bucket_index()

        Converts a key into a bucket index.

        Complexity:
            Average: O(1)
    */
        size_t bucket_index(
                const key_type& key,
                size_t bucket_count
        ) const {

            return hash_(key) % bucket_count;
        }


        size_t bucket_index(const key_type& key) const {

            return bucket_index(key, bucket_count_);
        }


    /*
        allocate_buckets()

        Allocates an array of bucket pointers and initializes
        every bucket to nullptr.

        Complexity:
            O(bucket_count)
    */
        static Node** allocate_buckets(size_t count) {

            Node** buckets = new Node*[count];

            for (size_t i = 0; i < count; ++i)
                buckets[i] = nullptr;

            return buckets;
        }


    /*
        find_node()

        Searches the linked list belonging to the key's bucket.

        Average:
            O(1)

        Worst case:
            O(n)
    */
        Node* find_node(const key_type& key) {

            size_t index = bucket_index(key);

            Node* current = buckets_[index];

            while (current != nullptr) {

                if (equal_(current->data.first, key))
                    return current;

                current = current->next;
            }

            return nullptr;
        }


        const Node* find_node(const key_type& key) const {

            size_t index = bucket_index(key);

            const Node* current = buckets_[index];

            while (current != nullptr) {

                if (equal_(current->data.first, key))
                    return current;

                current = current->next;
            }

            return nullptr;
        }


    /*
        ensure_capacity_for_insert()

        Checks whether inserting one more element would exceed
        the maximum load factor.

        If necessary, the number of buckets is doubled.

        load_factor = size / bucket_count
    */
        void ensure_capacity_for_insert() {

            float future_load =
                    static_cast<float>(size_ + 1) /
                    static_cast<float>(bucket_count_);

            if (future_load > max_load_factor_)
                rehash(bucket_count_ * 2);
        }


    /*
        destroy_nodes()

        Deletes every node currently stored in the map.

        Complexity:
            O(n)
    */
        void destroy_nodes() {

            for (size_t i = 0; i < bucket_count_; ++i) {

                Node* current = buckets_[i];

                while (current != nullptr) {

                    Node* next = current->next;

                    delete current;

                    current = next;
                }

                buckets_[i] = nullptr;
            }

            size_ = 0;
        }


    /*
        copy_from()

        Copies all elements from another HashMap.

        Complexity:
            O(n)
    */
        void copy_from(const HashMap& other) {

            for (size_t i = 0; i < other.bucket_count_; ++i) {

                const Node* current = other.buckets_[i];

                while (current != nullptr) {

                    insert(
                            current->data.first,
                            current->data.second
                    );

                    current = current->next;
                }
            }
        }


    public:


        // Iterator
        class iterator {

            friend class HashMap;

        private:

            HashMap* map_;
            size_t bucket_;
            Node* node_;


            iterator(
                    HashMap* map,
                    size_t bucket,
                    Node* node
            )
                    : map_(map),
                      bucket_(bucket),
                      node_(node) {}


        /*
            move_to_next_bucket()

            Searches for the next non-empty bucket.
        */
            void move_to_next_bucket() {

                ++bucket_;

                while (
                        bucket_ < map_->bucket_count_ &&
                        map_->buckets_[bucket_] == nullptr
                        ) {
                    ++bucket_;
                }

                if (bucket_ < map_->bucket_count_)
                    node_ = map_->buckets_[bucket_];
                else
                    node_ = nullptr;
            }


        public:

            using value_type = std::pair<const Key, T>;
            using reference = value_type&;
            using pointer = value_type*;


            iterator()
                    : map_(nullptr),
                      bucket_(0),
                      node_(nullptr) {}


            reference operator*() const {
                return node_->data;
            }


            pointer operator->() const {
                return &node_->data;
            }


            iterator& operator++() {

                if (node_->next != nullptr) {

                    node_ = node_->next;
                }
                else {

                    move_to_next_bucket();
                }

                return *this;
            }


            iterator operator++(int) {

                iterator temp = *this;

                ++(*this);

                return temp;
            }


            bool operator==(const iterator& other) const {

                return map_ == other.map_ &&
                       node_ == other.node_;
            }


            bool operator!=(const iterator& other) const {

                return !(*this == other);
            }
        };


        // Const Iterator
        class const_iterator {

            friend class HashMap;

        private:

            const HashMap* map_;
            size_t bucket_;
            const Node* node_;


            const_iterator(
                    const HashMap* map,
                    size_t bucket,
                    const Node* node
            )
                    : map_(map),
                      bucket_(bucket),
                      node_(node) {}


            void move_to_next_bucket() {

                ++bucket_;

                while (
                        bucket_ < map_->bucket_count_ &&
                        map_->buckets_[bucket_] == nullptr
                        ) {
                    ++bucket_;
                }

                if (bucket_ < map_->bucket_count_)
                    node_ = map_->buckets_[bucket_];
                else
                    node_ = nullptr;
            }


        public:

            using value_type = std::pair<const Key, T>;
            using reference = const value_type&;
            using pointer = const value_type*;


            const_iterator()
                    : map_(nullptr),
                      bucket_(0),
                      node_(nullptr) {}


            // iterator -> const_iterator
            const_iterator(const iterator& other)
                    : map_(other.map_),
                      bucket_(other.bucket_),
                      node_(other.node_) {}


            reference operator*() const {
                return node_->data;
            }


            pointer operator->() const {
                return &node_->data;
            }


            const_iterator& operator++() {

                if (node_->next != nullptr) {

                    node_ = node_->next;
                }
                else {

                    move_to_next_bucket();
                }

                return *this;
            }


            const_iterator operator++(int) {

                const_iterator temp = *this;

                ++(*this);

                return temp;
            }


            bool operator==(const const_iterator& other) const {

                return map_ == other.map_ &&
                       node_ == other.node_;
            }


            bool operator!=(const const_iterator& other) const {

                return !(*this == other);
            }
        };



        // Constructors / Rule of Five
        HashMap()
                : buckets_(allocate_buckets(DEFAULT_BUCKET_COUNT)),
                  bucket_count_(DEFAULT_BUCKET_COUNT),
                  size_(0),
                  max_load_factor_(1.0f),
                  hash_(),
                  equal_() {}


        explicit HashMap(size_t bucket_count)
                : buckets_(
                allocate_buckets(
                        bucket_count == 0
                        ? DEFAULT_BUCKET_COUNT
                        : bucket_count
                )
        ),
                  bucket_count_(
                          bucket_count == 0
                          ? DEFAULT_BUCKET_COUNT
                          : bucket_count
                  ),
                  size_(0),
                  max_load_factor_(1.0f),
                  hash_(),
                  equal_() {}


        ~HashMap() {

            destroy_nodes();

            delete[] buckets_;
        }


    /*
        Copy constructor

        Creates an independent copy of another HashMap.

        Complexity:
            O(n + bucket_count)
    */
        HashMap(const HashMap& other)
                : buckets_(allocate_buckets(other.bucket_count_)),
                  bucket_count_(other.bucket_count_),
                  size_(0),
                  max_load_factor_(other.max_load_factor_),
                  hash_(other.hash_),
                  equal_(other.equal_) {

            copy_from(other);
        }


    /*
        Copy assignment

        Uses copy-and-swap.

        Complexity:
            O(n)
    */
        HashMap& operator=(const HashMap& other) {

            if (this != &other) {

                HashMap temp(other);

                swap(temp);
            }

            return *this;
        }


    /*
        Move constructor

        Transfers ownership of the bucket array.

        Complexity:
            O(1)
    */
        HashMap(HashMap&& other) noexcept
                : buckets_(other.buckets_),
                  bucket_count_(other.bucket_count_),
                  size_(other.size_),
                  max_load_factor_(other.max_load_factor_),
                  hash_(std::move(other.hash_)),
                  equal_(std::move(other.equal_)) {

            other.buckets_ = nullptr;
            other.bucket_count_ = 0;
            other.size_ = 0;
        }


    /*
        Move assignment

        Releases current contents and takes ownership of the
        other HashMap's storage.

        Complexity:
            O(n) because current elements must be destroyed.
    */
        HashMap& operator=(HashMap&& other) noexcept {

            if (this != &other) {

                destroy_nodes();

                delete[] buckets_;

                buckets_ = other.buckets_;
                bucket_count_ = other.bucket_count_;
                size_ = other.size_;

                max_load_factor_ = other.max_load_factor_;

                hash_ = std::move(other.hash_);
                equal_ = std::move(other.equal_);

                other.buckets_ = nullptr;
                other.bucket_count_ = 0;
                other.size_ = 0;
            }

            return *this;
        }


        bool empty() const {
            return size_ == 0;
        }


        size_t size() const {
            return size_;
        }


        size_t bucket_count() const {
            return bucket_count_;
        }


    /*
        load_factor()

        Number of elements divided by number of buckets.

        Complexity:
            O(1)
    */
        float load_factor() const {

            if (bucket_count_ == 0)
                return 0.0f;

            return static_cast<float>(size_) /
                   static_cast<float>(bucket_count_);
        }


        float max_load_factor() const {
            return max_load_factor_;
        }


    /*
        Change the maximum allowed load factor.

        If the current table is already too full, rehash
        immediately.

        Complexity:
            O(1) normally
            O(n) if rehashing is required
    */
        void max_load_factor(float value) {

            if (value <= 0.0f)
                return;

            max_load_factor_ = value;

            if (load_factor() > max_load_factor_)
                reserve(size_);
        }

    /*
        operator[]

        Returns the mapped value associated with key.

        If the key does not exist, inserts:

            key -> T{}

        Average:
            O(1)

        Worst:
            O(n)
    */
        mapped_type& operator[](const key_type& key) {

            Node* existing = find_node(key);

            if (existing != nullptr)
                return existing->data.second;

            ensure_capacity_for_insert();

            size_t index = bucket_index(key);

            Node* node =
                    new Node(key, mapped_type{}, buckets_[index]);

            buckets_[index] = node;

            ++size_;

            return node->data.second;
        }


        mapped_type& operator[](key_type&& key) {

            Node* existing = find_node(key);

            if (existing != nullptr)
                return existing->data.second;

            ensure_capacity_for_insert();

            size_t index = bucket_index(key);

            Node* node =
                    new Node(
                            std::move(key),
                            mapped_type{},
                            buckets_[index]
                    );

            buckets_[index] = node;

            ++size_;

            return node->data.second;
        }


    /*
        at()

        Returns the mapped value for an existing key.

        Throws std::out_of_range if the key does not exist.

        Average:
            O(1)

        Worst:
            O(n)
    */
        mapped_type& at(const key_type& key) {

            Node* node = find_node(key);

            if (node == nullptr)
                throw std::out_of_range(
                        "HashMap::at - key not found"
                );

            return node->data.second;
        }


        const mapped_type& at(const key_type& key) const {

            const Node* node = find_node(key);

            if (node == nullptr)
                throw std::out_of_range(
                        "HashMap::at - key not found"
                );

            return node->data.second;
        }


        iterator find(const key_type& key) {

            size_t index = bucket_index(key);

            Node* node = buckets_[index];

            while (node != nullptr) {

                if (equal_(node->data.first, key))
                    return iterator(this, index, node);

                node = node->next;
            }

            return end();
        }


        const_iterator find(const key_type& key) const {

            size_t index = bucket_index(key);

            const Node* node = buckets_[index];

            while (node != nullptr) {

                if (equal_(node->data.first, key))
                    return const_iterator(this, index, node);

                node = node->next;
            }

            return end();
        }


        bool contains(const key_type& key) const {

            return find_node(key) != nullptr;
        }

    /*
        insert()

        Inserts a new key-value pair.

        If the key already exists, nothing is changed.

        Returns:
            true  -> insertion occurred
            false -> key already existed

        Average:
            O(1)

        Worst:
            O(n)
    */
        bool insert(
                const key_type& key,
                const mapped_type& value
        ) {

            if (contains(key))
                return false;

            ensure_capacity_for_insert();

            size_t index = bucket_index(key);

            Node* node =
                    new Node(key, value, buckets_[index]);

            buckets_[index] = node;

            ++size_;

            return true;
        }


        bool insert(
                key_type&& key,
                mapped_type&& value
        ) {

            if (contains(key))
                return false;

            ensure_capacity_for_insert();

            size_t index = bucket_index(key);

            Node* node =
                    new Node(
                            std::move(key),
                            std::move(value),
                            buckets_[index]
                    );

            buckets_[index] = node;

            ++size_;

            return true;
        }


    /*
        emplace()

        Constructs a mapped value and inserts it with the
        provided key.

        Average:
            O(1)

        Worst:
            O(n)
    */
        template<typename... Args>
        bool emplace(
                const key_type& key,
                Args&&... args
        ) {

            if (contains(key))
                return false;

            ensure_capacity_for_insert();

            size_t index = bucket_index(key);

            Node* node =
                    new Node(
                            key,
                            mapped_type(
                                    std::forward<Args>(args)...
                            ),
                            buckets_[index]
                    );

            buckets_[index] = node;

            ++size_;

            return true;
        }


    /*
        erase()

        Removes an element by key.

        Returns:
            true  -> element removed
            false -> key not found

        Average:
            O(1)

        Worst:
            O(n)
    */
        bool erase(const key_type& key) {

            size_t index = bucket_index(key);

            Node* current = buckets_[index];
            Node* previous = nullptr;

            while (current != nullptr) {

                if (equal_(current->data.first, key)) {

                    if (previous == nullptr)
                        buckets_[index] = current->next;
                    else
                        previous->next = current->next;

                    delete current;

                    --size_;

                    return true;
                }

                previous = current;
                current = current->next;
            }

            return false;
        }


    /*
        clear()

        Removes every element while keeping the bucket array.

        Complexity:
            O(n + bucket_count)
    */
        void clear() {

            destroy_nodes();
        }


    /*
        rehash()

        Changes the number of buckets and redistributes every
        existing node according to its new bucket index.

        Existing nodes are reused rather than recreated.

        Complexity:
            O(n + new_bucket_count)
    */
        void rehash(size_t new_bucket_count) {

            if (new_bucket_count == 0)
                new_bucket_count = 1;

            /*
                The table must still have enough buckets to respect
                max_load_factor.
            */
            size_t minimum =
                    static_cast<size_t>(
                            static_cast<float>(size_) /
                            max_load_factor_
                    );

            if (
                    static_cast<float>(minimum) *
                    max_load_factor_
                    < static_cast<float>(size_)
                    ) {
                ++minimum;
            }

            if (new_bucket_count < minimum)
                new_bucket_count = minimum;

            if (new_bucket_count == bucket_count_)
                return;


            Node** new_buckets =
                    allocate_buckets(new_bucket_count);


            for (size_t i = 0; i < bucket_count_; ++i) {

                Node* current = buckets_[i];

                while (current != nullptr) {

                    Node* next = current->next;

                    size_t new_index =
                            bucket_index(
                                    current->data.first,
                                    new_bucket_count
                            );

                    current->next =
                            new_buckets[new_index];

                    new_buckets[new_index] =
                            current;

                    current = next;
                }
            }


            delete[] buckets_;

            buckets_ = new_buckets;
            bucket_count_ = new_bucket_count;
        }


    /*
        reserve()

        Ensures enough buckets exist to store at least count
        elements without exceeding max_load_factor.

        Complexity:
            O(n) if rehashing occurs
            O(1) otherwise
    */
        void reserve(size_t count) {

            size_t required =
                    static_cast<size_t>(
                            static_cast<float>(count) /
                            max_load_factor_
                    );

            if (
                    static_cast<float>(required) *
                    max_load_factor_
                    < static_cast<float>(count)
                    ) {
                ++required;
            }

            if (required > bucket_count_)
                rehash(required);
        }


    /*
        swap()

        Exchanges all internal state with another HashMap.

        Complexity:
            O(1)
    */
        void swap(HashMap& other) {

            using std::swap;

            swap(buckets_, other.buckets_);
            swap(bucket_count_, other.bucket_count_);
            swap(size_, other.size_);

            swap(
                    max_load_factor_,
                    other.max_load_factor_
            );

            swap(hash_, other.hash_);
            swap(equal_, other.equal_);
        }


        // Iterators
        iterator begin() {

            for (size_t i = 0; i < bucket_count_; ++i) {

                if (buckets_[i] != nullptr)
                    return iterator(this, i, buckets_[i]);
            }

            return end();
        }


        iterator end() {

            return iterator(
                    this,
                    bucket_count_,
                    nullptr
            );
        }


        const_iterator begin() const {

            for (size_t i = 0; i < bucket_count_; ++i) {

                if (buckets_[i] != nullptr)
                    return const_iterator(
                            this,
                            i,
                            buckets_[i]
                    );
            }

            return end();
        }


        const_iterator end() const {

            return const_iterator(
                    this,
                    bucket_count_,
                    nullptr
            );
        }


        const_iterator cbegin() const {
            return begin();
        }


        const_iterator cend() const {
            return end();
        }
    };

} // namespace ministl