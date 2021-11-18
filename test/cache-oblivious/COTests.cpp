#include <gtest/gtest.h>
#include "common/geometryIO.h"

#include <kdtree/cache-oblivious/cokdtree.h>

#include "CO2DStructureTest.h"
#include "../shared/Shared2DTest.h"
#include "../shared/QueryTest.h"

static constexpr int dim = 2;
// <dim, objT, parallel, false>

// not parallel, not coarse
typedef CO_KdTree<dim, point<dim>, false, false> serialSingleTreeT;

INSTANTIATE_TYPED_TEST_SUITE_P(Serial, CO2DStructureTest, serialSingleTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(Serial_CO, Shared2DTest, serialSingleTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(Serial_CO, QueryTest, serialSingleTreeT);

// not parallel, coarse
typedef CO_KdTree<dim, point<dim>, false, true> serialCoarseTreeT;

INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_CO, Shared2DTest, serialCoarseTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(SerialCoarse_CO, QueryTest, serialCoarseTreeT);

// parallel, not coarse
typedef CO_KdTree<dim, point<dim>, true, false> parallelSingleTreet;

INSTANTIATE_TYPED_TEST_SUITE_P(Parallel, CO2DStructureTest, parallelSingleTreet);
INSTANTIATE_TYPED_TEST_SUITE_P(Parallel_CO, Shared2DTest, parallelSingleTreet);
INSTANTIATE_TYPED_TEST_SUITE_P(Parallel_CO, QueryTest, parallelSingleTreet);

// parallel, coarse
typedef CO_KdTree<dim, point<dim>, true, true> parallelCoarseTreeT;

INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_CO, Shared2DTest, parallelCoarseTreeT);
INSTANTIATE_TYPED_TEST_SUITE_P(ParallelCoarse_CO, QueryTest, parallelCoarseTreeT);
