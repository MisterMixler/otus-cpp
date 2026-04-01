#pragma once

#include "serialization/iserializer.h"

#include <fstream>

namespace editor {

/// Trivial line-oriented text serializer (one shape per line).
class FileSerializer : public ISerializer {
public:
    void save(const Document& doc, const std::string& path) override {
        std::ofstream out(path);
        for (const auto& s : doc.shapes())
            out << s->serialize() << '\n';
    }

    std::unique_ptr<Document> load(const std::string& path) override {
        auto doc = std::make_unique<Document>();
        std::ifstream in(path);
        // Stub: real implementation would parse each line and
        // reconstruct shapes via a factory.
        static_cast<void>(in);
        return doc;
    }
};

} // namespace editor
