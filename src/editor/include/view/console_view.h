#pragma once

#include "view/iview.h"
#include "model/document.h"

#include <iostream>

namespace editor {

/// Minimal console-based view for demonstration purposes.
class ConsoleView : public IView {
public:
    void render(const Document& doc) override {
        std::cout << "--- document (" << doc.size() << " shapes) ---\n";
        for (std::size_t i = 0; i < doc.size(); ++i)
            std::cout << "  [" << i << "] " << doc.shape_at(i).type() << '\n';
    }

    void show_message(const std::string& msg) override {
        std::cout << msg << '\n';
    }
};

} // namespace editor
