#pragma once

#include "model/ishape.h"
#include "model/point.h"

#include <sstream>

namespace editor {

class Line : public IShape {
public:
    Line(Point start, Point end)
        : start_(start), end_(end) {}

    std::string type() const override { return "line"; }

    std::string serialize() const override {
        std::ostringstream os;
        os << "line "
           << start_.x << ' ' << start_.y << ' '
           << end_.x   << ' ' << end_.y;
        return os.str();
    }

private:
    Point start_;
    Point end_;
};

} // namespace editor
