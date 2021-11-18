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

// Define another benchmark
template <int dim, class Tree>
static void bench_insert_query(benchmark::State& state) {
  auto size = state.range(0);
  auto batch_percentage = state.range(1);
  auto k = state.range(2);
  DSType ds_type = (DSType)state.range(3);

  auto points_ = BenchmarkDS<dim>(size, ds_type, false);
  const auto& points = points_;

  int log2size = (int)std::ceil(std::log2(points.size()));
  size_t div_size = points.size() * batch_percentage / 100;

  // benchmark
  for (auto _ : state) {
    {
      state.PauseTiming();
      Tree tree(log2size);
      state.ResumeTiming();

      // INSERT
      size_t pos = 0;
      while (pos < points.size()) {
        auto next_pos = std::min(pos + div_size, points.size());
        tree.insert(points.cut(pos, next_pos));
        pos = next_pos;
      }

      // QUERY
      tree.template knn<0, 0>(points, k);
      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Instantiate benchmarks
BENCH(insert_query, 2, COTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(insert_query, 2, BHLTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(insert_query, 2, LogTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});

BENCH(insert_query, 3, COTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_GEO_LIFE}});
BENCH(insert_query, 3, BHLTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_GEO_LIFE}});

BENCH(insert_query, 3, LogTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_GEO_LIFE}});

BENCH(insert_query, 5, COTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(insert_query, 5, BHLTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(insert_query, 5, LogTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(insert_query, 7, COTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(insert_query, 7, BHLTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(insert_query, 7, LogTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
