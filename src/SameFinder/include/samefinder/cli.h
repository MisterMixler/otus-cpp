#pragma once

namespace samefinder {

struct Options;

// Parses CLI args, prints help/errors to stdout/stderr, exits on --help or errors.
Options parse_args(int argc, char** argv);

} // namespace samefinder

