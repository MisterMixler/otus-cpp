#pragma once

#include <string>

namespace editor {

class Document;

/// Abstract view — knows how to present a document and user messages.
class IView {
public:
    virtual ~IView() = default;

    /// Render the current state of the document.
    virtual void render(const Document& doc) = 0;

    /// Display a status / diagnostic message.
    virtual void show_message(const std::string& msg) = 0;
};

} // namespace editor
