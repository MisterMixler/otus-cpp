#pragma once

#include "model/ishape.h"
#include "model/point.h"

#include <sstream>

namespace editor {

class Circle : public IShape {
public:
    Circle(Point center, double radius)
        : center_(center), radius_(radius) {}

    std::string type() const override { return "circle"; }

    std::string serialize() const override {
        std::ostringstream os;
        os << "circle " << center_.x << ' ' << center_.y << ' ' << radius_;
        return os.str();
    }

private:
    Point  center_;
    double radius_;
};

} // namespace editor
