#pragma once

#include <memory>
#include <string>

namespace editor {

/// Abstract base for every graphic primitive stored in a Document.
class IShape {
public:
    virtual ~IShape() = default;

    /// Human-readable type tag (e.g. "circle", "rectangle").
    virtual std::string type() const = 0;

    /// Serialise the shape into a single-line text representation.
    virtual std::string serialize() const = 0;
};

using ShapePtr = std::unique_ptr<IShape>;

} // namespace editor
