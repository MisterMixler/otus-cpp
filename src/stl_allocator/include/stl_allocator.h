#pragma once

#include <cstddef>
#include <memory>
#include <new>

template <typename T, std::size_t Capacity>
class PoolAllocator {
    static_assert(Capacity > 0, "Capacity must be greater than zero.");

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;

    template <typename U>
    struct rebind {
        using other = PoolAllocator<U, Capacity>;
    };

    PoolAllocator() : pool_(std::make_shared<Pool>()) {}

    PoolAllocator(const PoolAllocator& other) noexcept = default;

    template <typename U>
    PoolAllocator(const PoolAllocator<U, Capacity>&) noexcept
        : pool_(std::make_shared<Pool>()) {}

    PoolAllocator& operator=(const PoolAllocator& other) noexcept = default;

    pointer allocate(size_type n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > pool_->capacity - pool_->offset) {
            throw std::bad_alloc();
        }
        pointer result = pool_->data + pool_->offset;
        pool_->offset += n;
        return result;
    }

    void deallocate(pointer /*ptr*/, size_type /*n*/) noexcept {}

    size_type max_size() const noexcept {
        return Capacity;
    }

    bool operator==(const PoolAllocator& other) const noexcept {
        return pool_.get() == other.pool_.get();
    }

    bool operator!=(const PoolAllocator& other) const noexcept {
        return !(*this == other);
    }

private:
    struct Pool {
        Pool()
            : data(static_cast<T*>(::operator new(sizeof(T) * Capacity))),
              capacity(Capacity),
              offset(0) {}

        ~Pool() {
            ::operator delete(data);
        }

        T* data;
        size_type capacity;
        size_type offset;
    };

    std::shared_ptr<Pool> pool_;
};
