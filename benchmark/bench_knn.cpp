#include <iostream>
#include <algorithm>
#include "parlay/parallel.h"
#include "common/get_time.h"
#include "common/geometry.h"
#include "common/geometryIO.h"
#include "common/parse_command_line.h"

#include "kdtree/cache-oblivious/cokdtree.h"
#include "kdtree/binary-heap-layout/bhlkdtree.h"
#include "kdtree/log-tree/logtree.h"
#include "kdtree/shared/dual.h"

#include <benchmark/benchmark.h>
#include "./utils.h"

using namespace benchIO;

using coord = double;

// Arguments: {k_type, size, k}
// template <int dim, class Tree>
// static std::tuple<Tree, parlay::sequence<point<dim>>, int> unpack(benchmark::State& state) {
//// unpack args
// auto size = state.range(0);
// auto k = state.range(1);
// std::cout << "(size, k, dim) = (" << size << ", " << k << ", " << dim << ")" << std::endl;

//// construct tree
// parlay::sequence<point<dim>> points;
// points = BenchmarkDS<dim>(size);
// Tree tree(points);

// return {tree, points, k};
//}

// Define another benchmark
template <int dim, class Tree, int k_type>
static void bench_knn(benchmark::State& state) {
  auto size = state.range(0);
  auto k = state.range(1);
  DSType ds_type = (DSType)state.range(2);
  parlay::sequence<point<dim>> points;
  points = BenchmarkDS<dim>(size, ds_type);
  Tree tree(points);

  // benchmark
  for (auto _ : state) {
    RUN_AND_CLEAR((tree.template knn<(k_type & 2), (k_type & 1)>(points, k)));
  }
}

// Define another benchmark
template <int dim, int k_type>
static void bench_knn2(benchmark::State& state) {
  typedef LogTree_t<dim> Tree;
  auto size = state.range(0);
  auto k = state.range(1);
  DSType ds_type = (DSType)state.range(2);
  parlay::sequence<point<dim>> points;
  points = BenchmarkDS<dim>(size, ds_type);
  Tree tree(points);

  // benchmark
  for (auto _ : state) {
    RUN_AND_CLEAR((tree.template knn2<(k_type & 2), (k_type & 1)>(points, k)));
  }
}

// Define another benchmark
template <int dim, int k_type>
static void bench_knn3(benchmark::State& state) {
  typedef LogTree_t<dim> Tree;
  auto size = state.range(0);
  auto k = state.range(1);
  DSType ds_type = (DSType)state.range(2);
  parlay::sequence<point<dim>> points;
  points = BenchmarkDS<dim>(size, ds_type);
  Tree tree(points);

  // benchmark
  for (auto _ : state) {
    RUN_AND_CLEAR((tree.template knn3<(k_type & 2), (k_type & 1)>(points, k)));
  }
}

template <int dim, class Tree>
static void bench_dual_knn(benchmark::State& state) {
  // typedef LogTree_t<dim> Tree;
  auto size = state.range(0);
  auto k = state.range(1);
  DSType ds_type = (DSType)state.range(2);
  parlay::sequence<point<dim>> points;
  points = BenchmarkDS<dim>(size, ds_type);
  Tree tree(points);

  // benchmark
  for (auto _ : state) {
    RUN_AND_CLEAR(dualKnn(points, tree, k));
  }
}

// Instantiate benchmarks
BENCH(knn, 2, COTree_t<2>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(knn, 2, BHLTree_t<2>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(knn, 2, LogTree_t<2>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(knn2, 2, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(knn3, 2, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(dual_knn, 2, COTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(dual_knn, 2, BHLTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(dual_knn, 2, LogTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});

BENCH(knn, 3, COTree_t<3>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(knn, 3, BHLTree_t<3>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(knn, 3, LogTree_t<3>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(knn2, 3, 0)->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(knn3, 3, 0)->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(dual_knn, 3, COTree_t<3>)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(dual_knn, 3, BHLTree_t<3>)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});
BENCH(dual_knn, 3, LogTree_t<3>)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_GEO_LIFE}});

BENCH(knn, 5, COTree_t<5>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(knn, 5, BHLTree_t<5>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(knn, 5, LogTree_t<5>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(knn2, 5, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(knn3, 5, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(dual_knn, 5, COTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(dual_knn, 5, BHLTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(dual_knn, 5, LogTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(knn, 7, COTree_t<7>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(knn, 7, BHLTree_t<7>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(knn, 7, LogTree_t<7>, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(knn2, 7, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(knn3, 7, 0)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(dual_knn, 7, COTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(dual_knn, 7, BHLTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(dual_knn, 7, LogTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});

BENCH(knn, 10, COTree_t<10>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_HT}});
BENCH(knn, 10, BHLTree_t<10>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_HT}});
BENCH(knn3, 10, 0)->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_HT}});

BENCH(knn, 16, COTree_t<16>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_CHEM}});
BENCH(knn, 16, BHLTree_t<16>, 0)
    ->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_CHEM}});
BENCH(knn3, 16, 0)->ArgsProduct({{10'000'000}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {DS_CHEM}});
