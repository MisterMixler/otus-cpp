#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace samefinder {

enum class HashAlgorithm { Crc32, Md5 };

struct Options final {
  std::vector<std::string> scan_dirs;
  std::vector<std::string> exclude_dirs;
  std::size_t depth = 0;
  std::uintmax_t min_size = 1;
  std::vector<std::string> masks; // wildcard masks, case-insensitive
  std::size_t block_size = 4096;
  HashAlgorithm hash = HashAlgorithm::Crc32;
};

} // namespace samefinder

