#include "samefinder/cli.h"

#include "samefinder/options.h"

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace po = boost::program_options;

namespace samefinder {
namespace {

std::string to_lower_copy(std::string s) {
  boost::algorithm::to_lower(s);
  return s;
}

} // namespace

Options parse_args(int argc, char** argv) {
  Options opt;
  std::string hash;

  po::options_description desc("SameFinder options");
  desc.add_options()("help,h", "show help")(
      "scan,s", po::value<std::vector<std::string>>(&opt.scan_dirs)->multitoken()->required(),
      "directories to scan (one or more)")(
      "exclude,e",
      po::value<std::vector<std::string>>(&opt.exclude_dirs)->multitoken()->default_value({},
                                                                                         ""),
      "directories to exclude (one or more)")(
      "depth,d", po::value<std::size_t>(&opt.depth)->default_value(0),
      "scan depth (0 - only specified directories)")(
      "min-size,m", po::value<std::uintmax_t>(&opt.min_size)->default_value(1),
      "minimal file size in bytes (default: 1)")(
      "mask", po::value<std::vector<std::string>>(&opt.masks)->multitoken()->default_value({},
                                                                                           ""),
      "allowed file name masks, e.g. \"*.txt\" \"*.cpp\" (case-insensitive)")(
      "block-size,b", po::value<std::size_t>(&opt.block_size)->default_value(4096),
      "block size S for reading (bytes)")(
      "hash,H", po::value<std::string>(&hash)->default_value("crc32"),
      "hash algorithm: crc32 | md5");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      std::cout << desc << "\n";
      std::exit(0);
    }
    po::notify(vm);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n\n" << desc << "\n";
    std::exit(2);
  }

  if (opt.block_size == 0) {
    std::cerr << "error: block-size must be > 0\n";
    std::exit(2);
  }

  hash = to_lower_copy(hash);
  if (hash == "crc32") {
    opt.hash = HashAlgorithm::Crc32;
  } else if (hash == "md5") {
    opt.hash = HashAlgorithm::Md5;
  } else {
    std::cerr << "error: unsupported hash algorithm: " << hash << "\n";
    std::exit(2);
  }

  for (auto& m : opt.masks) {
    m = to_lower_copy(m);
  }

  return opt;
}

} // namespace samefinder

