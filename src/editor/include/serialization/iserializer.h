#pragma once

#include "model/document.h"

#include <memory>
#include <string>

namespace editor {

/// Strategy interface for document persistence.
class ISerializer {
public:
    virtual ~ISerializer() = default;

    /// Save the document to the given path.
    virtual void save(const Document& doc, const std::string& path) = 0;

    /// Load a document from the given path.
    virtual std::unique_ptr<Document> load(const std::string& path) = 0;
};

} // namespace editor
