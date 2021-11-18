#ifndef TEST_LOGTREE_LT2DDELETETEST_H
#define TEST_LOGTREE_LT2DDELETETEST_H

#include <gtest/gtest.h>
#include "common/geometryIO.h"

#include <kdtree/log-tree/logtree.h>

class Bulk {
 public:
  static constexpr bool bulk = true;
};
class NotBulk {
 public:
  static constexpr bool bulk = false;
};
typedef point<2> pointT;

template <typename Pair>
class LT2DDeleteTest : public ::testing::Test {
 public:
  const int DIM = 2;
};

TYPED_TEST_SUITE_P(LT2DDeleteTest);

TYPED_TEST_P(LT2DDeleteTest, Delete1) {
  using LTree = typename TypeParam::first_type;
  constexpr bool bulk = TypeParam::second_type::bulk;

  const char* test_file = "../resources/2d-UniformInSphere-1k.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  LTree tree;
  tree.insert(points);
  for (size_t i = 0; i < points.size(); i++) {
    ASSERT_TRUE(tree.contains(points[i]));
  }

  const auto num_to_remove = 70;
  parlay::sequence<pointT> to_remove(num_to_remove);
  for (size_t i = 0; i < num_to_remove; i++) {
    to_remove[i] = points[points.size() - 1 - i];
  }

  tree.template erase<bulk>(to_remove);

  for (int i = 0; i < (int)points.size(); i++) {
    if (i < (int)points.size() - num_to_remove)
      ASSERT_TRUE(tree.contains(points[i]));
    else
      ASSERT_FALSE(tree.contains(points[i]));
  }
}

TYPED_TEST_P(LT2DDeleteTest, Delete2) {
  using LTree = typename TypeParam::first_type;
  constexpr bool bulk = TypeParam::second_type::bulk;

  const char* test_file = "../resources/2d-UniformInSphere-1k.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  LTree tree;
  tree.insert(points);
  for (size_t i = 0; i < points.size(); i++) {
    ASSERT_TRUE(tree.contains(points[i]));
  }

  parlay::sequence<pointT> to_remove(points.size() / 2);
  for (size_t i = 0; i < points.size() / 2; i++) {
    to_remove[i] = points[2 * i];
  }

  tree.template erase<bulk>(to_remove);

  for (size_t i = 0; i < points.size(); i++) {
    if (i % 2 == 0)
      ASSERT_FALSE(tree.contains(points[i]));
    else
      ASSERT_TRUE(tree.contains(points[i]));
  }
}

// TODO: split this into a value parameterized test or something
TYPED_TEST_P(LT2DDeleteTest, BigDelete1) {
  using LTree = typename TypeParam::first_type;
  constexpr bool bulk = TypeParam::second_type::bulk;

  const char* test_file = "../resources/2d-UniformInSphere-10K.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  for (int skip = 2; skip < 10; skip++) {
    std::cout << " ----- TESTING " << skip << " ----------- " << std::endl;
    LTree tree;
    tree.insert(points);
    for (size_t i = 0; i < points.size(); i++) {
      ASSERT_TRUE(tree.contains(points[i]));
    }

    parlay::sequence<pointT> to_remove((points.size() + (skip - 1)) / skip);
    for (size_t i = 0; i < to_remove.size(); i++) {
      to_remove[i] = points[skip * i];
    }

    tree.template erase<bulk>(to_remove);

    auto check_present = [&](size_t i) { return tree.contains(points[i]); };
    auto check_missing = [&](size_t i) { return !tree.contains(points[i]); };
    for (size_t i = 0; i < points.size(); i++) {
      if (i % skip == 0)
        ASSERT_PRED1(check_missing, i);
      else
        ASSERT_PRED1(check_present, i);
    }
  }
}

TYPED_TEST_P(LT2DDeleteTest, BigDelete2) {
  using LTree = typename TypeParam::first_type;
  constexpr bool bulk = TypeParam::second_type::bulk;

  const char* test_file = "../resources/2d-UniformInSphere-10K.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  for (int skip = 2; skip < 10; skip++) {
    std::cout << " ----- TESTING " << skip << " ----------- " << std::endl;
    LTree tree;
    tree.insert(points);
    for (size_t i = 0; i < points.size(); i++) {
      ASSERT_TRUE(tree.contains(points[i]));
    }

    auto num_to_keep = (points.size() + (skip - 1)) / skip;
    auto num_to_remove = points.size() - num_to_keep;
    parlay::sequence<pointT> to_remove(num_to_remove);
    int j = 0;
    for (size_t i = 0; i < points.size(); i++) {
      if (i % skip == 0) continue;
      to_remove[j++] = points[i];
    }
    ASSERT_EQ(j, num_to_remove);  // sanity check

    tree.template erase<bulk>(to_remove);

    auto check_present = [&](size_t i) { return tree.contains(points[i]); };
    auto check_missing = [&](size_t i) { return !tree.contains(points[i]); };
    for (size_t i = 0; i < points.size(); i++) {
      if (i % skip == 0)
        ASSERT_PRED1(check_present, i);
      else
        ASSERT_PRED1(check_missing, i);
    }
  }
}

REGISTER_TYPED_TEST_SUITE_P(LT2DDeleteTest, Delete1, Delete2, BigDelete1, BigDelete2);
#endif  // TEST_LOGTREE_LT2DDELETETEST_H
