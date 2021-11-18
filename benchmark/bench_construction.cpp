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
static void bench_construction(benchmark::State& state) {
  auto size = state.range(0);
  auto percentage = state.range(1);
  DSType ds_type = (DSType)state.range(2);

  auto points_ = BenchmarkDS<dim>(size, ds_type);
  const auto& points = points_;
  auto point_slice = points.cut(0, (points.size() * percentage) / 100);

  // benchmark
  for (auto _ : state) {
    {
      Tree tree(point_slice);
      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Instantiate benchmarks
BENCH(construction, 2, COTree_t<2>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(construction, 2, BHLTree_t<2>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(construction, 2, LogTree_t<2>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});

BENCH(construction, 3, COTree_t<3>)->ArgsProduct({{10'000'000}, {100, 50}, {DS_GEO_LIFE}});
BENCH(construction, 3, BHLTree_t<3>)->ArgsProduct({{10'000'000}, {100, 50}, {DS_GEO_LIFE}});
BENCH(construction, 3, LogTree_t<3>)->ArgsProduct({{10'000'000}, {100, 50}, {DS_GEO_LIFE}});

BENCH(construction, 5, COTree_t<5>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(construction, 5, BHLTree_t<5>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(construction, 5, LogTree_t<5>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR}});

BENCH(construction, 7, COTree_t<7>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(construction, 7, BHLTree_t<7>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(construction, 7, LogTree_t<7>)
    ->ArgsProduct({{10'000'000}, {100, 50}, {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});

BENCH(construction, 10, COTree_t<10>)->ArgsProduct({{10'000'000}, {100}, {DS_HT}});
BENCH(construction, 10, BHLTree_t<10>)->ArgsProduct({{10'000'000}, {100}, {DS_HT}});
BENCH(construction, 10, LogTree_t<10>)->ArgsProduct({{10'000'000}, {100}, {DS_HT}});

BENCH(construction, 16, COTree_t<16>)->ArgsProduct({{10'000'000}, {100}, {DS_CHEM}});
BENCH(construction, 16, BHLTree_t<16>)->ArgsProduct({{10'000'000}, {100}, {DS_CHEM}});
BENCH(construction, 16, LogTree_t<16>)->ArgsProduct({{10'000'000}, {100}, {DS_CHEM}});
