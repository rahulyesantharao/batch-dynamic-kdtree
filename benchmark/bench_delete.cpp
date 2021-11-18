#include <iostream>
#include <algorithm>
#include "parlay/parallel.h"
#include "parlay/random.h"
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
static void bench_delete_(benchmark::State& state) {
  auto size = state.range(0);
  auto percentage = state.range(1);
  DSType ds_type = (DSType)state.range(2);

  parlay::sequence<point<dim>> points;
  points = BenchmarkDS<dim>(size, ds_type);

  // generate (deterministic) random sample to delete
  int del_size = (points.size() * percentage) / 100;
  auto to_delete = random_shuffle(points);  // random shuffle
  std::vector<parlay::sequence<point<dim>>> del_arr;
  size_t pos = 0;
  while (pos < points.size()) {
    auto next_pos = std::min(pos + del_size, points.size());
    parlay::sequence<point<dim>> pts;
    pts.assign(to_delete.cut(pos, next_pos));
    del_arr.push_back(std::move(pts));
    pos = next_pos;
  }

  // benchmark
  for (auto _ : state) {
    {
      state.PauseTiming();
      Tree tree(points);
      state.ResumeTiming();

      // actual benchmark
      for (size_t i = 0; i < del_arr.size(); i++) {
        tree.bulk_erase(del_arr[i]);
      }
      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Instantiate benchmarks
BENCH(delete_, 2, COTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(delete_, 2, BHLTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(delete_, 2, LogTree_t<2>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});

BENCH(delete_, 3, COTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});
BENCH(delete_, 3, BHLTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});
BENCH(delete_, 3, LogTree_t<3>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_GEO_LIFE}});

BENCH(delete_, 5, COTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(delete_, 5, BHLTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(delete_, 5, LogTree_t<5>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(delete_, 7, COTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(delete_, 7, BHLTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(delete_, 7, LogTree_t<7>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});

BENCH(delete_, 10, COTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});
BENCH(delete_, 10, BHLTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});
BENCH(delete_, 10, LogTree_t<10>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_HT}});

BENCH(delete_, 16, COTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
BENCH(delete_, 16, BHLTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
BENCH(delete_, 16, LogTree_t<16>)
    ->ArgsProduct({{10'000'000},
                   {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {DS_CHEM}});
