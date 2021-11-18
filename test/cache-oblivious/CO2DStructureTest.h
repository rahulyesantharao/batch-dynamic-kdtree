#ifndef TEST_CACHEOBLIVIOUS_CO2DSTRUCTURETEST_H
#define TEST_CACHEOBLIVIOUS_CO2DSTRUCTURETEST_H
#include <gtest/gtest.h>
#include "common/geometryIO.h"

#include <kdtree/cache-oblivious/cokdtree.h>
#include <kdtree/shared/macro.h>

#include "../shared/BasicStructure.h"

template <typename Tree>
class CO2DStructureTest : public BasicStructure2D<Tree> {
 public:
  static const int DIM = 2;
};

TYPED_TEST_SUITE_P(CO2DStructureTest);
TYPED_TEST_P(CO2DStructureTest, LayoutSize2) {
  // create the tree
  auto tree = this->CONSTRUCT_2D_SIZE_2();
  auto root = tree.root();  // get the root to verify the layout

  // Check node types
  ASSERT_FALSE(root[0].isLeaf());
  ASSERT_TRUE(root[1].isLeaf());
  ASSERT_TRUE(root[2].isLeaf());
  // Check point counts
  ASSERT_EQ(root[0].countPoints(), 2);
  ASSERT_EQ(root[1].countPoints(), 1);
  ASSERT_EQ(root[2].countPoints(), 1);
  // Check memory values
  ASSERT_EQ(root[0].getSplitDimension(), 0);
  //#ifdef USE_MEDIAN_SELECTION
  ASSERT_EQ(root[0].getSplitValue(), 1.0);
  //#else
  // ASSERT_EQ(root[0].getSplitValue(), 0.5);
  //#endif
  ASSERT_EQ(root[1].getValues().size(), 1);
  ASSERT_EQ(root[1].getValues()[0], this->POINT_ARR_2[0]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[1]), std::make_pair(0, 1));
  ASSERT_EQ(root[2].getValues().size(), 1);
  ASSERT_EQ(root[2].getValues()[0], this->POINT_ARR_2[1]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[2]), std::make_pair(1, 2));
  // Check memory layout
  ASSERT_EQ(root->getLeft(), root + 1);
  ASSERT_EQ(root->getRight(), root + 2);
}

TYPED_TEST_P(CO2DStructureTest, LayoutSize8) {
  // create the tree
  auto tree = this->CONSTRUCT_2D_SIZE_8();
  auto root = tree.root();  // get the root to verify the layout

  // Check node types

  /*               0
   *           1       2
   *         3   6   9   C
   *        4 5 7 8 A B D E
   */
  ASSERT_FALSE(root[0].isLeaf());
  ASSERT_FALSE(root[1].isLeaf());
  ASSERT_FALSE(root[2].isLeaf());

  ASSERT_FALSE(root[3].isLeaf());
  ASSERT_TRUE(root[4].isLeaf());
  ASSERT_TRUE(root[5].isLeaf());

  ASSERT_FALSE(root[6].isLeaf());
  ASSERT_TRUE(root[7].isLeaf());
  ASSERT_TRUE(root[8].isLeaf());

  ASSERT_FALSE(root[9].isLeaf());
  ASSERT_TRUE(root[10].isLeaf());
  ASSERT_TRUE(root[11].isLeaf());

  ASSERT_FALSE(root[12].isLeaf());
  ASSERT_TRUE(root[13].isLeaf());
  ASSERT_TRUE(root[14].isLeaf());

  // Check point counts
  ASSERT_EQ(root[0].countPoints(), 8);
  ASSERT_EQ(root[1].countPoints(), 4);
  ASSERT_EQ(root[2].countPoints(), 4);

  ASSERT_EQ(root[3].countPoints(), 2);
  ASSERT_EQ(root[4].countPoints(), 1);
  ASSERT_EQ(root[5].countPoints(), 1);

  ASSERT_EQ(root[6].countPoints(), 2);
  ASSERT_EQ(root[7].countPoints(), 1);
  ASSERT_EQ(root[8].countPoints(), 1);

  ASSERT_EQ(root[9].countPoints(), 2);
  ASSERT_EQ(root[10].countPoints(), 1);
  ASSERT_EQ(root[11].countPoints(), 1);

  ASSERT_EQ(root[12].countPoints(), 2);
  ASSERT_EQ(root[13].countPoints(), 1);
  ASSERT_EQ(root[14].countPoints(), 1);

  // Check memory values - serial case always uses median selection
  //#ifdef USE_MEDIAN_SELECTION
  double split_values[7] = {4, 2, 6, 1, 3, 5, 7};
  //#else
  // double split_values[7] = {3.5, 1.5, 5.5, 0.5, 2.5, 4.5, 6.5};
  //#endif
  // double split_values[7] = {4, 2, 6, 0.5, 2.5, 4.5, 6.5};

  ASSERT_EQ(root[0].getSplitDimension(), 0);
  ASSERT_EQ(root[0].getSplitValue(), split_values[0]);
  ASSERT_EQ(root[1].getSplitDimension(), 1);
  ASSERT_EQ(root[1].getSplitValue(), split_values[1]);
  ASSERT_EQ(root[2].getSplitDimension(), 1);
  ASSERT_EQ(root[2].getSplitValue(), split_values[2]);

  ASSERT_EQ(root[3].getSplitDimension(), 0);
  ASSERT_EQ(root[3].getSplitValue(), split_values[3]);
  ASSERT_EQ(root[4].getValues().size(), 1);
  ASSERT_EQ(root[4].getValues()[0], this->POINT_ARR_8[0]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[4]), std::make_pair(0, 1));
  ASSERT_EQ(root[5].getValues().size(), 1);
  ASSERT_EQ(root[5].getValues()[0], this->POINT_ARR_8[1]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[5]), std::make_pair(1, 2));

  ASSERT_EQ(root[6].getSplitDimension(), 0);
  ASSERT_EQ(root[6].getSplitValue(), split_values[4]);
  ASSERT_EQ(root[7].getValues().size(), 1);
  ASSERT_EQ(root[7].getValues()[0], this->POINT_ARR_8[2]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[7]), std::make_pair(2, 3));
  ASSERT_EQ(root[8].getValues().size(), 1);
  ASSERT_EQ(root[8].getValues()[0], this->POINT_ARR_8[3]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[8]), std::make_pair(3, 4));

  ASSERT_EQ(root[9].getSplitDimension(), 0);
  ASSERT_EQ(root[9].getSplitValue(), split_values[5]);
  ASSERT_EQ(root[10].getValues().size(), 1);
  ASSERT_EQ(root[10].getValues()[0], this->POINT_ARR_8[4]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[10]), std::make_pair(4, 5));
  ASSERT_EQ(root[11].getValues().size(), 1);
  ASSERT_EQ(root[11].getValues()[0], this->POINT_ARR_8[5]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[11]), std::make_pair(5, 6));

  ASSERT_EQ(root[12].getSplitDimension(), 0);
  ASSERT_EQ(root[12].getSplitValue(), split_values[6]);
  ASSERT_EQ(root[13].getValues().size(), 1);
  ASSERT_EQ(root[13].getValues()[0], this->POINT_ARR_8[6]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[13]), std::make_pair(6, 7));
  ASSERT_EQ(root[14].getValues().size(), 1);
  ASSERT_EQ(root[14].getValues()[0], this->POINT_ARR_8[7]);
  ASSERT_EQ(tree.getNodeValueIdx(&root[14]), std::make_pair(7, 8));

  // Check memory layout
  ASSERT_EQ(root[0].getLeft(), root + 1);
  ASSERT_EQ(root[0].getRight(), root + 2);
  ASSERT_EQ(root[1].getLeft(), root + 3);
  ASSERT_EQ(root[1].getRight(), root + 6);
  ASSERT_EQ(root[2].getLeft(), root + 9);
  ASSERT_EQ(root[2].getRight(), root + 12);

  ASSERT_EQ(root[3].getLeft(), root + 4);
  ASSERT_EQ(root[3].getRight(), root + 5);
  ASSERT_EQ(root[6].getLeft(), root + 7);
  ASSERT_EQ(root[6].getRight(), root + 8);
  ASSERT_EQ(root[9].getLeft(), root + 10);
  ASSERT_EQ(root[9].getRight(), root + 11);
  ASSERT_EQ(root[12].getLeft(), root + 13);
  ASSERT_EQ(root[12].getRight(), root + 14);
}

REGISTER_TYPED_TEST_SUITE_P(CO2DStructureTest, LayoutSize2, LayoutSize8);
#endif  // TEST_CACHEOBLIVIOUS_CO2DSTRUCTURETEST_H
