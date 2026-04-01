/**
 * @file main.cpp
 * @brief Entry point and GUI callback stubs for the vector editor.
 *
 * All functions that would be wired to a real GUI toolkit are collected
 * here together with main(), as required by the assignment.
 */

#include "controller/controller.h"
#include "model/circle.h"
#include "model/line.h"
#include "model/rectangle.h"
#include "serialization/file_serializer.h"
#include "view/console_view.h"

#include <iostream>
#include <memory>

// ---- GUI callback stubs ------------------------------------------------

static editor::Controller& ctrl();

void on_new_document() {
    ctrl().new_document();
}

void on_import_document(const std::string& path) {
    ctrl().import_document(path);
}

void on_export_document(const std::string& path) {
    ctrl().export_document(path);
}

void on_add_circle(editor::Point center, double radius) {
    ctrl().add_shape(std::make_unique<editor::Circle>(center, radius));
}

void on_add_rectangle(editor::Point tl, editor::Point br) {
    ctrl().add_shape(std::make_unique<editor::Rectangle>(tl, br));
}

void on_add_line(editor::Point start, editor::Point end) {
    ctrl().add_shape(std::make_unique<editor::Line>(start, end));
}

void on_remove_shape(std::size_t index) {
    ctrl().remove_shape(index);
}

// ---- Singleton accessor for the demo -----------------------------------

static editor::Controller& ctrl() {
    static editor::Controller instance{
        std::make_shared<editor::ConsoleView>(),
        std::make_shared<editor::FileSerializer>()};
    return instance;
}

// ---- main --------------------------------------------------------------

int main() {
    on_new_document();

    on_add_circle({10.0, 20.0}, 5.0);
    on_add_rectangle({0.0, 0.0}, {100.0, 50.0});
    on_add_line({0.0, 0.0}, {30.0, 40.0});

    on_remove_shape(1);

    on_export_document("demo.txt");
    on_import_document("demo.txt");

    return 0;
}
