#ifndef TEST_LOGTREE_LT2DSTRUCTURETEST_H
#define TEST_LOGTREE_LT2DSTRUCTURETEST_H

#include <gtest/gtest.h>
#include "common/geometryIO.h"

#include <kdtree/log-tree/logtree.h>

typedef point<2> pointT;
pointT constructPoint(double d) {
  constexpr int dim = 2;
  double point_buf[dim];
  for (int k = 0; k < dim; k++) {
    point_buf[k] = d;
  }
  return pointT(point_buf);
}

template <typename LTree>
class LT2DStructureTest : public ::testing::Test {
 public:
  const int DIM = 2;
};

TYPED_TEST_SUITE_P(LT2DStructureTest);

TYPED_TEST_P(LT2DStructureTest, LayoutSize32) {
  TypeParam tree;

  // build over 32 points
  constexpr int buf_size = 4;
  for (int i = 0; i < 32 / buf_size; i++) {
    parlay::sequence<pointT> x(buf_size);
    for (int j = 0; j < buf_size; j++) {
      x[j] = constructPoint((double)(buf_size * i + j));
    }

    tree.insert(x);
  }

  // verify over 32 points
  auto tree_mask = tree.getTreeMask();
  if (TypeParam::coarsen_)
    ASSERT_EQ(tree_mask, 0);
  else
    ASSERT_EQ(tree_mask, 0b0011);  // lazy shifting -> will leave full buffer

  for (int i = 0; i < 32; i++) {
    ASSERT_TRUE(tree.contains(constructPoint((double)i)));
  }
  for (int i = 32; i < 100; i++) {
    ASSERT_FALSE(tree.contains(constructPoint((double)i)));
  }
}

TYPED_TEST_P(LT2DStructureTest, LayoutSize64) {
  TypeParam tree;

  // build over 64 points
  for (int i = 0; i < 32; i++) {
    parlay::sequence<pointT> x(2);
    for (int j = 0; j < 2; j++) {
      x[j] = constructPoint((double)(2 * i + j));
    }

    tree.insert(x);
  }

  // verify over 64 points
  auto tree_mask = tree.getTreeMask();
  if (TypeParam::coarsen_)
    ASSERT_EQ(tree_mask, 0b01);
  else
    ASSERT_EQ(tree_mask, 0b0111);  // lazy shifting -> won't move full buffer

  for (int i = 0; i < 64; i++) {
    ASSERT_TRUE(tree.contains(constructPoint((double)i)));
  }
  for (int i = 64; i < 200; i++) {
    ASSERT_FALSE(tree.contains(constructPoint((double)i)));
  }
}

TYPED_TEST_P(LT2DStructureTest, Verify) {
  const char* test_file = "../resources/2d-UniformInSphere-1k.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  TypeParam tree;
  tree.insert(points);

  // check trees
  if (TypeParam::coarsen_)
    ASSERT_EQ(tree.getTreeMask(), 0b11111);
  else
    ASSERT_EQ(tree.getTreeMask(), 0b1111101);

  // positive tests
  for (const auto& p : points) {
    ASSERT_TRUE(tree.contains(p));
  }

  // negative tests
  for (int i = 0; i < 100; i++) {
    ASSERT_FALSE(tree.contains(constructPoint((double)i)));
  }
}

// Come up with a better abstraction; this is a direct copy of BasicKnn test
TYPED_TEST_P(LT2DStructureTest, BasicKnn2) {
  // construct tree
  const char* test_file = "../resources/2d-UniformInSphere-1k.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  TypeParam tree;
  tree.insert(points);

  ASSERT_EQ(tree.size(), points.size());

  // construct brute force solution
  constexpr int k = 4;
  auto check = knnBuf::bruteforceKnn(points, k);
  ASSERT_EQ(check.size(), k * points.size());

  for (int i = 0; i < 4; i++) {
    decltype(check) res;
    if (i == 0) {
      res = tree.template knn2<false, false>(points, k);
    } else if (i == 1) {
      res = tree.template knn2<false, true>(points, k);
    } else if (i == 2) {
      res = tree.template knn2<true, false>(points, k);
    } else {
      res = tree.template knn2<true, true>(points, k);
    }

    // verify result
    ASSERT_EQ(res.size(), k * points.size());
    for (size_t i = 0; i < points.size(); i++) {
      // std::cout << "CHECKING knn FOR " << i << ": " << points[i] << std::endl;
      // sort the results for this point
      auto res_start = res.begin() + i * k;
      auto check_start = check.begin() + i * k;
      auto compare = [&](const pointT* l, const pointT* r) {
        return l->coordinate(0) < r->coordinate(0);
      };
      std::sort(check_start, check_start + k, compare);
      std::sort(res_start, res_start + k, compare);

      for (int j = 0; j < k; j++) {
        auto res_pt = **(res_start + j);
        auto check_pt = **(check_start + j);
        // std::cout << " - " << j << ": " << res_pt << " vs " << check_pt << std::endl;
        EXPECT_EQ(res_pt, check_pt) << "dist res, dist check =  " << points[i].dist(res_pt) << ", "
                                    << points[i].dist(check_pt);
      }
    }
  }
}

// Come up with a better abstraction; this is a direct copy of BasicKnn test
TYPED_TEST_P(LT2DStructureTest, BasicKnn3) {
  // construct tree
  const char* test_file = "../resources/2d-UniformInSphere-1k.pbbs";
  int check_dim = readDimensionFromFile(test_file);
  ASSERT_EQ(check_dim, this->DIM);
  auto points = readPointsFromFile<pointT>(test_file);

  TypeParam tree;
  tree.insert(points);

  ASSERT_EQ(tree.size(), points.size());

  // construct brute force solution
  constexpr int k = 4;
  auto check = knnBuf::bruteforceKnn(points, k);
  ASSERT_EQ(check.size(), k * points.size());

  for (int i = 0; i < 4; i++) {
    decltype(check) res;
    if (i == 0) {
      res = tree.template knn3<false, false>(points, k);
    } else if (i == 1) {
      res = tree.template knn3<false, true>(points, k);
    } else if (i == 2) {
      res = tree.template knn3<true, false>(points, k);
    } else {
      res = tree.template knn3<true, true>(points, k);
    }

    // verify result
    ASSERT_EQ(res.size(), k * points.size());
    for (size_t i = 0; i < points.size(); i++) {
      // std::cout << "CHECKING knn FOR " << i << ": " << points[i] << std::endl;
      // sort the results for this point
      auto res_start = res.begin() + i * k;
      auto check_start = check.begin() + i * k;
      auto compare = [&](const pointT* l, const pointT* r) {
        return l->coordinate(0) < r->coordinate(0);
      };
      std::sort(check_start, check_start + k, compare);
      std::sort(res_start, res_start + k, compare);

      for (int j = 0; j < k; j++) {
        auto res_pt = **(res_start + j);
        auto check_pt = **(check_start + j);
        // std::cout << " - " << j << ": " << res_pt << " vs " << check_pt << std::endl;
        EXPECT_EQ(res_pt, check_pt) << "dist res, dist check =  " << points[i].dist(res_pt) << ", "
                                    << points[i].dist(check_pt);
      }
    }
  }
}

REGISTER_TYPED_TEST_SUITE_P(
    LT2DStructureTest, LayoutSize32, LayoutSize64, Verify, BasicKnn2, BasicKnn3);

#endif  // TEST_LOGTREE_LT2DSTRUCTURETEST_H
