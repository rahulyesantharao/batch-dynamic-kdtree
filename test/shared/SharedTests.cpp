#include <set>
#include <gtest/gtest.h>
#include "common/geometryIO.h"
#include "kdtree/shared/box.h"
#include "kdtree/shared/bloom.h"
#include "BasicStructure.h"

class SharedTests : public ::testing::Test {};

TEST_F(SharedTests, BoundingBoxDistance) {
  auto ret1 =
      BoundingBoxDistance(point<2>({0, 0}), point<2>({1, 1}), point<2>({4, 5}), point<2>({6, 6}));
  ASSERT_EQ(ret1, 5);
}

TEST_F(SharedTests, BloomFilter) {
  parlay::sequence<point<2>> points;
  for (int i = 0; i < 100000; i++) {
    points.push_back(point<2>({(double)i, (double)i}));
  }

  // build bloom filter
  auto to_insert = KEEP_EVEN(points);
  BloomFilter<2> bf(to_insert.size());
  bf.build(to_insert);

  // query bloom filter
  auto filtered = bf.filter(points);
  std::cout << "to_insert.size(), points.size() -> filtered.size() = " << to_insert.size() << ", "
            << points.size() << " -> " << filtered.size() << std::endl;

  auto round = [](double d) { return (int)(d + 0.5); };
  std::set<int> filtered_points;
  for (const auto& pt : filtered) {
    filtered_points.insert(round(pt.coordinate(0)));
  }

  for (const auto& ipt : to_insert) {
    ASSERT_EQ(filtered_points.count(round(ipt.coordinate(0))), 1)
        << "missing point " << ipt.coordinate(0) << " rounded -> " << round(ipt.coordinate(0));
  }
}
