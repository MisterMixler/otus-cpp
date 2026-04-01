#pragma once

#include "model/document.h"
#include "serialization/iserializer.h"
#include "view/iview.h"

#include <memory>
#include <string>

namespace editor {

/// Central MVC controller — mediates between Model, View and Serializer.
class Controller {
public:
    Controller(std::shared_ptr<IView> view,
               std::shared_ptr<ISerializer> serializer)
        : document_(std::make_unique<Document>()),
          view_(std::move(view)),
          serializer_(std::move(serializer)) {}

    void new_document() {
        document_ = std::make_unique<Document>();
        view_->show_message("new document created");
        view_->render(*document_);
    }

    void import_document(const std::string& path) {
        document_ = serializer_->load(path);
        view_->show_message("imported from " + path);
        view_->render(*document_);
    }

    void export_document(const std::string& path) {
        serializer_->save(*document_, path);
        view_->show_message("exported to " + path);
    }

    void add_shape(ShapePtr shape) {
        document_->add_shape(std::move(shape));
        view_->render(*document_);
    }

    void remove_shape(std::size_t index) {
        document_->remove_shape(index);
        view_->render(*document_);
    }

    [[nodiscard]] const Document& document() const noexcept {
        return *document_;
    }

private:
    std::unique_ptr<Document>    document_;
    std::shared_ptr<IView>       view_;
    std::shared_ptr<ISerializer> serializer_;
};

} // namespace editor
