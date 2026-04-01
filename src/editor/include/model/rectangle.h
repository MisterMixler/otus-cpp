#pragma once

#include "model/ishape.h"
#include "model/point.h"

#include <sstream>

namespace editor {

class Rectangle : public IShape {
public:
    Rectangle(Point top_left, Point bottom_right)
        : top_left_(top_left), bottom_right_(bottom_right) {}

    std::string type() const override { return "rectangle"; }

    std::string serialize() const override {
        std::ostringstream os;
        os << "rectangle "
           << top_left_.x     << ' ' << top_left_.y << ' '
           << bottom_right_.x << ' ' << bottom_right_.y;
        return os.str();
    }

private:
    Point top_left_;
    Point bottom_right_;
};

} // namespace editor
