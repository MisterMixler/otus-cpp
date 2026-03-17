#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

template <typename T, typename Alloc = std::allocator<T>>
class SimpleContainer {
private:
    struct Node {
        template <typename... Args>
        explicit Node(Args&&... args)
            : value(std::forward<Args>(args)...), next(nullptr) {}

        T value;
        Node* next;
    };

    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeTraits = std::allocator_traits<NodeAlloc>;

public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() : node_(nullptr) {}
        explicit iterator(Node* node) : node_(node) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.node_ == rhs.node_;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return lhs.node_ != rhs.node_;
        }

    private:
        Node* node_;
        friend class SimpleContainer;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() : node_(nullptr) {}
        explicit const_iterator(const Node* node) : node_(node) {}
        const_iterator(const iterator& other) : node_(other.node_) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        const_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
            return lhs.node_ == rhs.node_;
        }

        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
            return lhs.node_ != rhs.node_;
        }

    private:
        const Node* node_;
        friend class SimpleContainer;
    };

    SimpleContainer()
        : head_(nullptr),
          tail_(nullptr),
          size_(0),
          alloc_(NodeAlloc()) {}

    explicit SimpleContainer(const allocator_type& alloc)
        : head_(nullptr),
          tail_(nullptr),
          size_(0),
          alloc_(alloc) {}

    ~SimpleContainer() { clear(); }

    SimpleContainer(const SimpleContainer&) = delete;
    SimpleContainer& operator=(const SimpleContainer&) = delete;

    SimpleContainer(SimpleContainer&& other) noexcept
        : head_(other.head_),
          tail_(other.tail_),
          size_(other.size_),
          alloc_(std::move(other.alloc_)) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    SimpleContainer& operator=(SimpleContainer&& other) noexcept {
        if (this != &other) {
            clear();
            alloc_ = std::move(other.alloc_);
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        Node* node = NodeTraits::allocate(alloc_, 1);
        try {
            NodeTraits::construct(alloc_, node, std::forward<Args>(args)...);
        } catch (...) {
            NodeTraits::deallocate(alloc_, node, 1);
            throw;
        }
        append_node(node);
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end() const { return const_iterator(nullptr); }

    const_iterator cbegin() const { return const_iterator(head_); }
    const_iterator cend() const { return const_iterator(nullptr); }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }

    void clear() {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            NodeTraits::destroy(alloc_, current);
            NodeTraits::deallocate(alloc_, current, 1);
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

private:
    void append_node(Node* node) {
        if (!tail_) {
            head_ = node;
            tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    Node* head_;
    Node* tail_;
    size_type size_;
    NodeAlloc alloc_;
};
