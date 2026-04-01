#pragma once

#include "model/ishape.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace editor {

/// Aggregate model: an ordered collection of shapes that make up a drawing.
class Document {
public:
    void add_shape(ShapePtr shape) {
        shapes_.push_back(std::move(shape));
    }

    void remove_shape(std::size_t index) {
        if (index >= shapes_.size())
            throw std::out_of_range("shape index out of range");
        shapes_.erase(shapes_.begin() + static_cast<std::ptrdiff_t>(index));
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return shapes_.size();
    }

    [[nodiscard]] const IShape& shape_at(std::size_t index) const {
        return *shapes_.at(index);
    }

    [[nodiscard]] const std::vector<ShapePtr>& shapes() const noexcept {
        return shapes_;
    }

    void clear() noexcept { shapes_.clear(); }

private:
    std::vector<ShapePtr> shapes_;
};

} // namespace editor
