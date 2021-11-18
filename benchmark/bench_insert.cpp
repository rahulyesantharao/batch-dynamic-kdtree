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
static void bench_insert(benchmark::State& state) {
  auto size = state.range(0);
  auto batch_percentage = state.range(1);
  DSType ds_type = (DSType)state.range(2);

  auto points_ = BenchmarkDS<dim>(size, ds_type);
  const auto& points = points_;

  int log2size = (int)std::ceil(std::log2(points.size()));
  size_t div_size = points.size() * batch_percentage / 100;

  // benchmark
  for (auto _ : state) {
    {
      state.PauseTiming();
      Tree tree(log2size);
      state.ResumeTiming();

      // actual benchmark
      size_t pos = 0;
      while (pos < points.size()) {
        auto next_pos = std::min(pos + div_size, points.size());
        tree.insert(points.cut(pos, next_pos));
        pos = next_pos;
      }

      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Instantiate benchmarks
BENCH(insert, 2, COTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(insert, 2, BHLTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(insert, 2, LogTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});

BENCH(insert, 3, COTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});
BENCH(insert, 3, BHLTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});
BENCH(insert, 3, LogTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});

BENCH(insert, 5, COTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(insert, 5, BHLTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(insert, 5, LogTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(insert, 7, COTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(insert, 7, BHLTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(insert, 7, LogTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});

BENCH(insert, 10, COTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});
BENCH(insert, 10, BHLTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});
BENCH(insert, 10, LogTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});

BENCH(insert, 16, COTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
BENCH(insert, 16, BHLTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
BENCH(insert, 16, LogTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
