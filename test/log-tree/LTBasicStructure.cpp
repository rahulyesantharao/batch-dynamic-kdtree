#include <gtest/gtest.h>
#include "common/geometryIO.h"

#include <kdtree/log-tree/logtree.h>

#include "LT2DStructureTest.h"
#include "LT2DDeleteTest.h"
#include "../shared/QueryTest.h"

static constexpr int dim = 2;
static constexpr int NUM_TREES = 14;
static constexpr int BUFFER_LOG2_SIZE = 3;
typedef point<dim> pointT;

// serial, not coarse
typedef LogTree<NUM_TREES, BUFFER_LOG2_SIZE, dim, pointT, false, false> serialSingleTreeT;
typedef std::pair<serialSingleTreeT, NotBulk> SSNoBulk;
typedef std::pair<serialSingleTreeT, Bulk> SSBulk;

INSTANTIATE_TYPED_TEST_SUITE_P(Serial, LT2DStructureTest, serialSingleTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialNoBulk, LT2DDeleteTest, SSNoBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialBulk, LT2DDeleteTest, SSBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(Serial_LT, QueryTest, serialSingleTreeT);

// serial, coarse
typedef LogTree<NUM_TREES, 5, dim, pointT, false, true>
    serialCoarseTreeT;  // use 5 so that smallest static tree is not degenerate
typedef std::pair<serialCoarseTreeT, NotBulk> SCNoBulk;
typedef std::pair<serialCoarseTreeT, Bulk> SCBulk;

INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_LT, LT2DStructureTest, serialCoarseTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_LT_NB, LT2DDeleteTest, SCNoBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_LT_B, LT2DDeleteTest, SCBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_LT, QueryTest, serialCoarseTreeT);

// parallel, not coarse
typedef LogTree<NUM_TREES, BUFFER_LOG2_SIZE, dim, pointT, true, false> parallelSingleTreeT;
typedef std::pair<parallelSingleTreeT, NotBulk> PSNoBulk;
typedef std::pair<parallelSingleTreeT, Bulk> PSBulk;

INSTANTIATE_TYPED_TEST_SUITE_P(Parallel, LT2DStructureTest, parallelSingleTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelNoBulk, LT2DDeleteTest, PSNoBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelBulk, LT2DDeleteTest, PSBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(Parallel_LT, QueryTest, parallelSingleTreeT);

// parallel, coarse
typedef LogTree<NUM_TREES, 5, dim, pointT, true, true>
    parallelCoarseTreeT;  // use 5 so that smallest static tree is not degenerate
typedef std::pair<parallelCoarseTreeT, NotBulk> PCNoBulk;
typedef std::pair<parallelCoarseTreeT, Bulk> PCBulk;

INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_LT, LT2DStructureTest, parallelCoarseTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_LT_NB, LT2DDeleteTest, PCNoBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_LT_B, LT2DDeleteTest, PCBulk);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_LT, QueryTest, parallelCoarseTreeT);
