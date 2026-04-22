#include "samefinder/same_finder.h"

#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fs = boost::filesystem;

namespace samefinder {
namespace {

// Very small wildcard matcher supporting '*' and '?', case-insensitive.
bool wildcard_match_ci(std::string_view pattern, std::string_view text) {
  auto lower = [](char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  };

  std::size_t p = 0, t = 0;
  std::optional<std::size_t> star_p;
  std::size_t star_t = 0;

  while (t < text.size()) {
    if (p < pattern.size() &&
        (pattern[p] == '?' || lower(pattern[p]) == lower(text[t]))) {
      ++p;
      ++t;
      continue;
    }
    if (p < pattern.size() && pattern[p] == '*') {
      star_p = p++;
      star_t = t;
      continue;
    }
    if (star_p) {
      p = *star_p + 1;
      t = ++star_t;
      continue;
    }
    return false;
  }
  while (p < pattern.size() && pattern[p] == '*') {
    ++p;
  }
  return p == pattern.size();
}

bool is_under_dir(const fs::path& p, const fs::path& dir) {
  // Works with lexically_normal paths (no symlinks resolution).
  auto pn = p.lexically_normal();
  auto dn = dir.lexically_normal();

  auto pit = pn.begin();
  auto dit = dn.begin();
  for (; dit != dn.end(); ++dit, ++pit) {
    if (pit == pn.end() || *pit != *dit) {
      return false;
    }
  }
  return true;
}

std::vector<fs::path> normalize_dirs(const std::vector<std::string>& raw) {
  std::vector<fs::path> out;
  out.reserve(raw.size());
  for (const auto& s : raw) {
    fs::path p{s};
    try {
      p = fs::absolute(p).lexically_normal();
    } catch (...) {
      p = p.lexically_normal();
    }
    out.push_back(std::move(p));
  }
  return out;
}

struct BlockHasher {
  HashAlgorithm algo = HashAlgorithm::Crc32;

  std::array<std::uint32_t, 4> hash_block(const char* data, std::size_t n) const {
    if (algo == HashAlgorithm::Crc32) {
      boost::crc_32_type crc;
      crc.process_bytes(data, n);
      return {crc.checksum(), 0u, 0u, 0u};
    }

    boost::uuids::detail::md5 md5;
    md5.process_bytes(data, n);
    boost::uuids::detail::md5::digest_type digest{};
    md5.get_digest(digest);
    return {digest[0], digest[1], digest[2], digest[3]};
  }
};

using BlockHash = std::array<std::uint32_t, 4>;

struct BlockHashHasher {
  std::size_t operator()(const BlockHash& h) const noexcept {
    // Simple 64-bit mix. Good enough for bucketization.
    std::size_t x = 0xcbf29ce484222325ULL;
    for (std::uint32_t v : h) {
      x ^= static_cast<std::size_t>(v) + 0x9e3779b97f4a7c15ULL + (x << 6) + (x >> 2);
    }
    return x;
  }
};

struct FileReader {
  fs::path path;
  std::uintmax_t size = 0;
  std::size_t block_size = 0;
  std::size_t blocks_total = 0;
  BlockHasher hasher;

  std::ifstream stream;
  std::vector<BlockHash> block_hashes; // computed lazily

  FileReader(fs::path p, std::uintmax_t sz, std::size_t bs, BlockHasher h)
      : path(std::move(p)), size(sz), block_size(bs), hasher(h) {
    blocks_total = static_cast<std::size_t>((size + block_size - 1) / block_size);
    block_hashes.reserve(blocks_total);
  }

  const BlockHash& ensure_block_hash(std::size_t block_index) {
    if (block_index < block_hashes.size()) {
      return block_hashes[block_index];
    }

    if (!stream.is_open()) {
      stream.open(path.string(), std::ios::binary);
      if (!stream) {
        throw std::runtime_error("failed to open file: " + path.string());
      }
    }

    std::vector<char> buf(block_size, 0);

    while (block_hashes.size() <= block_index) {
      std::size_t idx = block_hashes.size();
      const std::uintmax_t offset = static_cast<std::uintmax_t>(idx) * block_size;
      std::size_t need = block_size;
      if (offset + need > size) {
        if (offset >= size) {
          need = 0;
        } else {
          need = static_cast<std::size_t>(size - offset);
        }
      }

      if (need > 0) {
        stream.read(buf.data(), static_cast<std::streamsize>(need));
        const auto got = static_cast<std::size_t>(stream.gcount());
        if (got != need) {
          throw std::runtime_error("failed to read file: " + path.string());
        }
        if (need < block_size) {
          std::fill(buf.begin() + static_cast<std::ptrdiff_t>(need), buf.end(), 0);
        }
      } else {
        std::fill(buf.begin(), buf.end(), 0);
      }

      block_hashes.push_back(hasher.hash_block(buf.data(), buf.size()));
    }

    return block_hashes[block_index];
  }
};

using FileId = std::size_t;

struct Group {
  std::vector<FileId> ids;
};

} // namespace

std::vector<CandidateFile> collect_candidates(const Options& opt) {
  const auto scan_dirs = normalize_dirs(opt.scan_dirs);
  const auto exclude_dirs = normalize_dirs(opt.exclude_dirs);

  std::vector<CandidateFile> out;

  for (const auto& root : scan_dirs) {
    if (!fs::exists(root) || !fs::is_directory(root)) {
      continue;
    }

    boost::system::error_code iter_ec;
    fs::recursive_directory_iterator it(root, iter_ec), end;
    if (iter_ec) {
      continue;
    }

    while (it != end) {
      auto advance = [&]() {
        it.increment(iter_ec);
        if (iter_ec) {
          iter_ec.clear();
        }
      };

      const fs::path p = it->path();

      // Depth limiting: iterator.depth() is 0 for direct children of root.
      if (it.depth() >= static_cast<int>(opt.depth) && fs::is_directory(p)) {
        it.disable_recursion_pending();
      }

      // Exclude dirs: if current path is under excluded dir, skip recursion and entry.
      bool excluded = false;
      for (const auto& ex : exclude_dirs) {
        if (!ex.empty() && is_under_dir(p, ex)) {
          excluded = true;
          if (fs::is_directory(p)) {
            it.disable_recursion_pending();
          }
          break;
        }
      }
      if (excluded) {
        advance();
        continue;
      }

      if (!fs::is_regular_file(p)) {
        advance();
        continue;
      }

      boost::system::error_code fs_ec;
      const auto sz = fs::file_size(p, fs_ec);
      if (fs_ec) {
        advance();
        continue;
      }
      if (sz < opt.min_size) {
        advance();
        continue;
      }
      if (!opt.masks.empty()) {
        const std::string filename = p.filename().string();
        bool ok = false;
        for (const auto& mask : opt.masks) {
          if (wildcard_match_ci(mask, filename)) {
            ok = true;
            break;
          }
        }
        if (!ok) {
          advance();
          continue;
        }
      }

      out.push_back(CandidateFile{fs::absolute(p).lexically_normal(), sz});
      advance();
    }
  }

  return out;
}

std::vector<std::vector<fs::path>> find_duplicates(const Options& opt,
                                                   const std::vector<CandidateFile>& files) {
  BlockHasher hasher;
  hasher.algo = opt.hash;

  // Group by size first.
  std::unordered_map<std::uintmax_t, std::vector<FileId>> by_size;
  by_size.reserve(files.size());
  for (FileId i = 0; i < files.size(); ++i) {
    by_size[files[i].size].push_back(i);
  }

  std::vector<FileReader> readers;
  readers.reserve(files.size());
  for (const auto& f : files) {
    readers.emplace_back(f.path, f.size, opt.block_size, hasher);
  }

  std::vector<std::vector<fs::path>> result;

  for (auto& kv : by_size) {
    auto& ids = kv.second;
    if (ids.size() < 2) {
      continue;
    }

    const std::uintmax_t size = kv.first;
    const std::size_t blocks_total =
        static_cast<std::size_t>((size + opt.block_size - 1) / opt.block_size);

    std::vector<Group> groups;
    groups.push_back(Group{ids});

    for (std::size_t b = 0; b < blocks_total; ++b) {
      std::vector<Group> next;
      next.reserve(groups.size());

      for (auto& g : groups) {
        if (g.ids.size() < 2) {
          continue;
        }

        std::unordered_map<BlockHash, std::vector<FileId>, BlockHashHasher> buckets;
        buckets.reserve(g.ids.size());

        for (FileId id : g.ids) {
          const auto& key = readers[id].ensure_block_hash(b);
          buckets[key].push_back(id);
        }

        for (auto& bkv : buckets) {
          auto& bucket_ids = bkv.second;
          if (bucket_ids.size() > 1) {
            next.push_back(Group{std::move(bucket_ids)});
          }
        }
      }

      groups = std::move(next);
      if (groups.empty()) {
        break;
      }
    }

    for (const auto& g : groups) {
      if (g.ids.size() < 2) {
        continue;
      }
      std::vector<fs::path> paths;
      paths.reserve(g.ids.size());
      for (FileId id : g.ids) {
        paths.push_back(readers[id].path);
      }
      std::sort(paths.begin(), paths.end());
      result.push_back(std::move(paths));
    }
  }

  std::sort(result.begin(), result.end(),
            [](const auto& a, const auto& b) { return a.front().string() < b.front().string(); });
  return result;
}

void print_groups(const std::vector<std::vector<fs::path>>& groups) {
  bool first_group = true;
  for (const auto& g : groups) {
    if (!first_group) {
      std::cout << "\n";
    }
    first_group = false;

    for (const auto& p : g) {
      std::cout << p.string() << "\n";
    }
  }
}

} // namespace samefinder

