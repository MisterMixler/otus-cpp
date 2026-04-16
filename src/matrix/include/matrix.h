#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <map>
#include <tuple>
#include <utility>

template<typename T, T Default, std::size_t Dims = 2>
class Matrix {
public:
    using Key = std::array<std::size_t, Dims>;

private:
    std::map<Key, T> data_;

    class LeafProxy {
        Matrix& matrix_;
        Key key_;

    public:
        LeafProxy(Matrix& m, Key k) : matrix_(m), key_(k) {}

        operator T() const {
            auto it = matrix_.data_.find(key_);
            return it != matrix_.data_.end() ? it->second : Default;
        }

        LeafProxy& operator=(const T& value) {
            if (value == Default)
                matrix_.data_.erase(key_);
            else
                matrix_.data_[key_] = value;
            return *this;
        }

        LeafProxy& operator=(const LeafProxy& other) {
            return *this = T(other);
        }
    };

    template<std::size_t Depth>
    class Proxy {
        Matrix& matrix_;
        std::array<std::size_t, Depth> indices_;

    public:
        Proxy(Matrix& m, std::array<std::size_t, Depth> idx)
            : matrix_(m), indices_(idx) {}

        auto operator[](std::size_t index) {
            std::array<std::size_t, Depth + 1> next{};
            std::copy(indices_.begin(), indices_.end(), next.begin());
            next[Depth] = index;
            if constexpr (Depth + 1 < Dims)
                return Proxy<Depth + 1>(matrix_, next);
            else
                return LeafProxy(matrix_, next);
        }
    };

    template<std::size_t... Is>
    static auto to_tuple(const Key& key, const T& value,
                         std::index_sequence<Is...>) {
        return std::make_tuple(key[Is]..., value);
    }

public:
    auto operator[](std::size_t index) {
        if constexpr (Dims == 1)
            return LeafProxy(*this, Key{index});
        else
            return Proxy<1>(*this, std::array<std::size_t, 1>{index});
    }

    std::size_t size() const { return data_.size(); }

    class Iterator {
        typename std::map<Key, T>::const_iterator it_;

    public:
        Iterator(typename std::map<Key, T>::const_iterator it) : it_(it) {}

        auto operator*() const {
            return to_tuple(it_->first, it_->second,
                            std::make_index_sequence<Dims>{});
        }

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator& other) const {
            return it_ != other.it_;
        }
    };

    Iterator begin() const { return Iterator(data_.cbegin()); }
    Iterator end() const { return Iterator(data_.cend()); }
};
