#include <gtest/gtest.h>

#include "samefinder/options.h"
#include "samefinder/same_finder.h"

#include <boost/filesystem.hpp>

#include <fstream>
#include <set>
#include <string>
#include <vector>

namespace fs = boost::filesystem;

namespace {

void write_file(const fs::path& p, const std::string& contents) {
  fs::create_directories(p.parent_path());
  std::ofstream out(p.string(), std::ios::binary);
  ASSERT_TRUE(out) << "failed to open for write: " << p.string();
  out << contents;
  ASSERT_TRUE(out.good()) << "failed to write: " << p.string();
}

std::string group_signature(const std::vector<fs::path>& g) {
  std::vector<std::string> names;
  names.reserve(g.size());
  for (const auto& p : g) {
    names.push_back(p.filename().string());
  }
  std::sort(names.begin(), names.end());
  std::string s;
  for (const auto& n : names) {
    if (!s.empty()) {
      s += '|';
    }
    s += n;
  }
  return s;
}

} // namespace

TEST(SameFinder, FindsTwoDuplicateGroups) {
  const fs::path root = fs::absolute(fs::unique_path(fs::temp_directory_path() / "samefinder_test_%%%%-%%%%"));

  // Two independent duplicate groups (same sizes, different contents).
  write_file(root / "g1" / "a.txt", "aaa");
  write_file(root / "g1" / "b.txt", "aaa");

  write_file(root / "g2" / "a.txt", "bbb");
  write_file(root / "g2" / "b.txt", "bbb");

  // Noise: empty directory should not affect results.
  fs::create_directories(root / "empty_dir");

  samefinder::Options opt;
  opt.scan_dirs = {root.string()};
  opt.exclude_dirs = {};
  // Files live one level below `g1/` and `g2/`, so depth must allow recursion.
  opt.depth = 10;
  opt.min_size = 1;
  opt.masks = {"*.txt"};
  opt.block_size = 5;
  opt.hash = samefinder::HashAlgorithm::Crc32;

  const auto candidates = samefinder::collect_candidates(opt);
  ASSERT_EQ(candidates.size(), 4u);

  const auto groups = samefinder::find_duplicates(opt, candidates);
  ASSERT_EQ(groups.size(), 2u);

  std::multiset<std::string> got;
  for (const auto& g : groups) {
    ASSERT_EQ(g.size(), 2u);
    got.insert(group_signature(g));
  }

  const std::multiset<std::string> expected{"a.txt|b.txt", "a.txt|b.txt"};
  EXPECT_EQ(got, expected);

  fs::remove_all(root);
}
