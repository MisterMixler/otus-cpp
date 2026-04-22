#include "samefinder/cli.h"
#include "samefinder/same_finder.h"

int main(int argc, char** argv) {
  const samefinder::Options opt = samefinder::parse_args(argc, argv);
  const auto candidates = samefinder::collect_candidates(opt);
  const auto groups = samefinder::find_duplicates(opt, candidates);
  samefinder::print_groups(groups);
  return 0;
}
