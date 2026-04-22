#pragma once

#include <boost/filesystem/path.hpp>

#include <cstdint>
#include <vector>

#include "samefinder/options.h"

namespace samefinder {

struct CandidateFile final {
  boost::filesystem::path path;
  std::uintmax_t size = 0;
};

// Scans directories and returns files matching filters (size/masks/exclude/depth).
std::vector<CandidateFile> collect_candidates(const Options& opt);

// Returns groups of identical files. Each group contains absolute normalized paths.
std::vector<std::vector<boost::filesystem::path>> find_duplicates(
    const Options& opt,
    const std::vector<CandidateFile>& files);

// Prints groups to stdout as required (one file per line, groups separated by empty line).
void print_groups(const std::vector<std::vector<boost::filesystem::path>>& groups);

} // namespace samefinder

