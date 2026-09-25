/*
    MiniSTL - List Implementation
    ------------------------------
    A simplified implementation of std::list for educational purposes.

    Features:
    - Doubly linked list
    - Rule of Five (copy/move semantics)
    - Bidirectional iterators
    - O(1) push_front / push_back
    - O(1) insert / erase with iterator

    Complexity guarantees follow std::list where possible.

    Author: Adam Abu Saleh
*/

#pragma once
#include <cstddef>     // size_t, ptrdiff_t
#include <iterator>    // iterator tags
#include <utility>     // std::move, std::swap
#include <type_traits> // remove_pointer_t
#include <cassert>  //assert

namespace ministl {

/*
    List<T>
    A doubly linked list container similar to std::list.
    Properties:
    - non-contiguous memory
    - constant time insertion and deletion
    - bidirectional iteration
*/
    template<typename T>
    class List {

    private:

    /*
        Internal node structure.
        Each node stores:
        - the element value
        - pointer to previous node
        - pointer to next node
    */
        struct Node {
            T value;
            Node* prev;
            Node* next;

            template<typename... Args>
            explicit Node(Args&&... args)
                    : value(std::forward<Args>(args)...),
                      prev(nullptr),
                      next(nullptr) {}
        };

        // pointer to first element
        Node* head_;

        // pointer to last element
        Node* tail_;

        // number of elements stored
        size_t size_;

    public:

    /*
        Bidirectional iterator for List.
        Internally wraps a Node pointer and walks through the linked nodes.
        Supports bidirectional traversal required for std::list-like containers.
    */
        template<typename Ptr>
        class ListIterator {

        private:
            Node* node_;
            Node* tail_;

        public:

            using value_type = std::remove_cv_t<std::remove_pointer_t<Ptr>>;
            using difference_type = std::ptrdiff_t;
            using pointer = Ptr;
            using reference = std::remove_pointer_t<Ptr>&;
            using iterator_category = std::bidirectional_iterator_tag;


            explicit ListIterator(Node* node = nullptr, Node* tail = nullptr)
                    : node_(node), tail_(tail) {}

            template<typename P>
            requires std::is_convertible_v<P, Ptr>
            ListIterator(const ListIterator<P>& other)
                    : node_(other.node_), tail_(other.tail_) {}

            reference operator*() const {
                assert(node_);
                return node_->value;
            }

            pointer operator->() const {
                assert(node_);
                return &node_->value;
            }

            // prefix increment
            ListIterator& operator++() {
                assert(node_);
                node_ = node_->next;
                return *this;
            }

            // postfix increment
            ListIterator operator++(int) {
                assert(node_);
                ListIterator tmp = *this;
                node_ = node_->next;
                return tmp;
            }

            ListIterator& operator--() {
                if (node_)
                    node_ = node_->prev;
                else
                    node_ = tail_;

                assert(node_);
                return *this;
            }

            ListIterator operator--(int) {
                ListIterator tmp = *this;
                --(*this);
                return tmp;
            }

            template<typename P>
            bool operator==(const ListIterator<P>& other) const {
                return node_ == other.node_;
            }

            template<typename P>
            bool operator!=(const ListIterator<P>& other) const {
                return node_ != other.node_;
            }

        private:
            template<typename>
            friend class ListIterator;

            friend class List;
        };

        using iterator = ListIterator<T*>;
        using const_iterator = ListIterator<const T*>;

        // default constructor
        List() : head_(nullptr), tail_(nullptr), size_(0) {}

    /*
        Copy constructor.

        Performs a deep copy of all nodes.
        If copying an element throws, already-created nodes
        are destroyed before the exception is rethrown.

        Complexity: O(n)
    */
        List(const List& other)
                : head_(nullptr), tail_(nullptr), size_(0) {

            try {
                for (Node* curr = other.head_; curr; curr = curr->next)
                    push_back(curr->value);
            }
            catch (...) {
                clear();
                throw;
            }
        }

        // move constructor: transfers ownership of node chain without copying elements
        List(List&& other) noexcept
                : head_(other.head_), tail_(other.tail_), size_(other.size_) {
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }

    /*
        Copy assignment using copy-and-swap.

        Creates the copy first so that if copying fails,
        the current list remains unchanged.

        Complexity: O(n)
    */
        List& operator=(const List& other) {
            if (this == &other)
                return *this;

            List temp(other);
            swap(temp);

            return *this;
        }

        // move assignment
        List& operator=(List&& other) noexcept {
            if(this == &other)
                return *this;

            clear();

            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        // destructor
        ~List() { clear(); }

        bool empty() const noexcept{ return size_ == 0; }

        size_t size() const noexcept{ return size_; }


        T& front() noexcept{
            assert(!empty());
            return head_->value;
        }

        const T& front() const noexcept{
            assert(!empty());
            return head_->value;
        }

        T& back() noexcept{
            assert(!empty());
            return tail_->value;
        }

        const T& back() const noexcept{
            assert(!empty());
            return tail_->value;
        }

    /*
        Appends element to the end of the list.

        Complexity: O(1)
    */
        void push_back(const T& value) {
            emplace_back(value);
        }

        // move version
        void push_back(T&& value) {
            emplace_back(std::move(value));
        }
    /*
        Constructs an element directly at the end of the list.

        Complexity: O(1)
    */
        template<typename... Args>
        T& emplace_back(Args&&... args) {
            Node* node = new Node(std::forward<Args>(args)...);

            if (empty()) {
                head_ = tail_ = node;
            }
            else {
                tail_->next = node;
                node->prev = tail_;
                tail_ = node;
            }

            ++size_;
            return node->value;
        }

    /*
        Constructs an element directly at the beginning of the list.

        Complexity: O(1)
    */
        template<typename... Args>
        T& emplace_front(Args&&... args) {
            Node* node = new Node(std::forward<Args>(args)...);

            if (empty()) {
                head_ = tail_ = node;
            }
            else {
                node->next = head_;
                head_->prev = node;
                head_ = node;
            }

            ++size_;
            return node->value;
        }

    /*
        Inserts element at the beginning.
        Complexity: O(1)
    */
        void push_front(const T& value) {
            emplace_front(value);
        }

        void push_front(T&& value) {
            emplace_front(std::move(value));
        }

    /*
        Removes last element.
        Complexity: O(1)
    */
        void pop_back() {
            if(empty()) return;
            Node* tmp = tail_;
            tail_ = tail_->prev;
            if(tail_)
                tail_->next = nullptr;
            else
                head_ = nullptr;
            delete tmp;
            size_--;
        }

    /*
        Removes first element.
        Complexity: O(1)
    */
        void pop_front() {
            if(empty()) return;
            Node* tmp = head_;
            head_ = head_->next;
            if(head_)
                head_->prev = nullptr;
            else
                tail_ = nullptr;
            delete tmp;
            size_--;
        }

    /*
        Removes all elements from the list.

        Complexity: O(n)
    */
        void clear() {
            Node* curr = head_;
            while(curr) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
        }

    /*
        Resizes the list to contain count elements.

        New elements are default-constructed when the list grows.

        Complexity: O(|count - size|)
    */
        void resize(size_t count) {
            while (size_ > count)
                pop_back();

            while (size_ < count)
                emplace_back();
        }

    /*
        Resizes the list to contain count elements.

        If the list grows, new elements are copies of value.
        If the list shrinks, elements are removed from the back.

        Complexity: O(|count - size|)
    */
        void resize(size_t count, const T& value) {
            while (size_ > count)
                pop_back();

            while (size_ < count)
                emplace_back(value);
        }

    /*
        Inserts element before iterator position.

        Complexity: O(1)
    */
        iterator insert(const_iterator pos, const T& value) {
            return emplace(pos, value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return emplace(pos, std::move(value));
        }

    /*
        Constructs an element directly before pos.

        Complexity: O(1)
    */
        template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            if (pos.node_ == nullptr) {
                emplace_back(std::forward<Args>(args)...);
                return iterator(tail_, tail_);
            }

            Node* curr = pos.node_;
            Node* node = new Node(std::forward<Args>(args)...);

            node->next = curr;
            node->prev = curr->prev;

            if (curr->prev)
                curr->prev->next = node;
            else
                head_ = node;

            curr->prev = node;

            ++size_;
            return iterator(node, tail_);
        }
    /*
        Removes the element at pos and returns an iterator
        to the element following the erased element.

        Complexity: O(1)
    */
        iterator erase(const_iterator pos) {
            Node* curr = pos.node_;

            if (!curr)
                return end();

            Node* next = curr->next;

            if (curr->prev)
                curr->prev->next = curr->next;
            else
                head_ = curr->next;

            if (curr->next)
                curr->next->prev = curr->prev;
            else
                tail_ = curr->prev;

            delete curr;
            --size_;

            return iterator(next, tail_);
        }


        iterator begin() { return iterator(head_, tail_); }
        iterator end() { return iterator(nullptr, tail_); }

        const_iterator begin() const { return const_iterator(head_, tail_); }
        const_iterator end() const { return const_iterator(nullptr, tail_); }

        const_iterator cbegin() const { return const_iterator(head_, tail_); }
        const_iterator cend() const { return const_iterator(nullptr, tail_); }

        void swap(List& other) noexcept{
            std::swap(head_, other.head_);
            std::swap(tail_, other.tail_);
            std::swap(size_, other.size_);
        }

    /*
        Removes all elements equal to value.

        Complexity: O(n)
    */
        void remove(const T& value) {
            remove_if([&value](const T& element) {
                return element == value;
            });
        }

        //list.remove_if([](int x){ return x % 2 == 0; });
        template<typename Pred>
        void remove_if(Pred pred){
            auto it = begin();
            while(it != end()){
                if(pred(*it))
                    it = erase(it);
                else
                    ++it;
            }
        }

        void reverse(){
            Node* curr = head_;
            while(curr){
                std::swap(curr->next, curr->prev);
                curr = curr->prev;
            }
            std::swap(head_, tail_);
        }

    /*
        Removes consecutive elements for which pred returns true.

        Complexity: O(n)
    */
        template<typename BinaryPred>
        void unique(BinaryPred pred) {
            if (empty())
                return;

            auto it = begin();
            auto next = it;
            ++next;

            while (next != end()) {
                if (pred(*it, *next))
                    next = erase(next);
                else {
                    it = next;
                    ++next;
                }
            }
        }

        void unique() {
            unique([](const T& a, const T& b) {
                return a == b;
            });
        }

        void splice(const_iterator pos, List& other) {
            if (this == &other || other.empty())
                return;

            if (pos.node_ == nullptr) {
                if (empty()) {
                    head_ = other.head_;
                    tail_ = other.tail_;
                }
                else {
                    tail_->next = other.head_;
                    other.head_->prev = tail_;
                    tail_ = other.tail_;
                }
            }
            else {
                Node* curr = pos.node_;
                Node* before = curr->prev;

                if (before)
                    before->next = other.head_;
                else
                    head_ = other.head_;

                other.head_->prev = before;
                other.tail_->next = curr;
                curr->prev = other.tail_;
            }

            size_ += other.size_;

            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }

    private:
        Node* split(Node* head){
            Node* slow = head;
            Node* fast = head;
            while(fast->next && fast->next->next){
                slow = slow->next;
                fast = fast->next->next;
            }
            Node* second = slow->next;
            slow->next = nullptr;
            if(second)
                second->prev = nullptr;
            return second;
        }

        template<typename Compare>
        Node* merge(Node* a, Node* b, Compare comp)
        {
            if(!a) return b;
            if(!b) return a;
            if(!comp(b->value, a->value)){
                a->next = merge(a->next, b, comp);
                if(a->next)
                    a->next->prev = a;
                a->prev = nullptr;
                return a;
            }
            else{
                b->next = merge(a, b->next, comp);
                if(b->next) b->next->prev = b;
                b->prev = nullptr;
                return b;
            }
        }

        template<typename Compare>
        Node* merge_sort(Node* node, Compare comp){
            if(!node || !node->next)
                return node;
            Node* second = split(node);
            node = merge_sort(node, comp);
            second = merge_sort(second, comp);
            return merge(node, second, comp);
        }

    public:
    /*
        Sorts the list using merge sort.
        Complexity: O(n log n)
    */
        void sort(){
            sort([](const T& a, const T& b){
                return a < b;
            });
        }

        template<typename Compare>
        void sort(Compare comp)
        {
            if(size_ < 2)
                return;
            head_ = merge_sort(head_, comp);
            tail_ = head_;
            while(tail_->next)
                tail_ = tail_->next;
        }
    };
} // namespace ministl