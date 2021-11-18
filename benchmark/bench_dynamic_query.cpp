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

//#define SHOW_TIMINGS
#define SMALL_BATCH_DELETE

// Define another benchmark
template <int dim, class Tree, bool log>
static void bench_dynamic_query(benchmark::State& state) {
  auto size = state.range(0);
  auto batch_percentage = state.range(1);
  auto k = state.range(2);
  DSType ds_type = (DSType)state.range(3);

  auto points_ = BenchmarkDS<dim>(size, ds_type, false);
  const auto& points = points_;

  int log2size = (int)std::ceil(std::log2(points.size()));
  size_t div_size = (points.size() * batch_percentage) / 100;
  size_t initial_div_size = (points.size() * 0.5) / 100;

  constexpr size_t sub_division = 4;
  auto del_points = random_shuffle(points);
#ifdef SMALL_BATCH_DELETE
  std::vector<parlay::sequence<point<dim>>> del_arr[sub_division];
  size_t del_pos = 0;
  for (size_t sd = 1; sd < sub_division; sd++) {
    while (del_pos < (points.size() * sd) / sub_division) {
      auto next_pos = std::min(del_pos + initial_div_size, points.size());
      parlay::sequence<point<dim>> pts;
      pts.assign(del_points.cut(del_pos, next_pos));
      del_arr[sd].push_back(std::move(pts));
      del_pos = next_pos;
    }
  }
#else
  std::vector<parlay::sequence<point<dim>>> del_arr;
  for (size_t sd = 1; sd < sub_division; sd++) {
    parlay::sequence<point<dim>> pts;
    pts.assign(del_points.cut((points.size() * (sd - 1)) / sub_division,
                              (points.size() * sd) / sub_division));
    del_arr.push_back(std::move(pts));
  }
#endif

  // benchmark
  for (auto _ : state) {
    {
      state.PauseTiming();
      Tree tree(log2size);
      state.ResumeTiming();

      timer t("[dyn iq]");
      auto timestamp = [&](std::string name, size_t idx) {
        auto ts = t.get_next();
#ifdef SHOW_TIMINGS
        std::cout << "[dyn iq] " << name << "_" << idx << " DONE: " << ts << std::endl;
#endif
        state.counters[name + "_" + std::to_string(idx)] = ts;
      };

      // INSERT
      size_t pos = 0;
      for (size_t sd = 0; sd < sub_division; sd++) {
        while (pos < (points.size() * (sd + 1)) / 4) {
          auto next_pos = std::min(pos + initial_div_size, points.size());
          tree.insert(points.cut(pos, next_pos));
          pos = next_pos;
        }
        timestamp("insert", sd);

        // QUERY
        if (log)
          tree.template knn3<0, 0>(points, k);
        else
          tree.template knn<0, 0>(points, k);
        timestamp("knn", sd);
      }

      // DELETE
      for (size_t sd = 1; sd < sub_division; sd++) {
#ifdef SMALL_BATCH_DELETE
        for (auto& d : del_arr[sd]) {
          tree.bulk_erase(d);
        }
#else
        tree.bulk_erase(del_arr[sd - 1]);
#endif
        timestamp("delete", sd);

        // QUERY
        if (log)
          tree.template knn3<0, 0>(points, k);
        else
          tree.template knn<0, 0>(points, k);
        timestamp("knn", sd + sub_division - 1);
      }

      state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Instantiate benchmarks
BENCH(dynamic_query, 2, BHLTree_t<2>, false)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 5},
                   //{10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {5},
                   //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
BENCH(dynamic_query, 2, LogTree_t<2>, true)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 5},
                   //{10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {5},
                   //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_UNIFORM_SPHERE, DS_VISUAL_VAR}});
// BENCH(dynamic_query, 3)
//->ArgsProduct({{10'000'000},
//{10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
//{5},
////{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
//{DS_GEO_LIFE}});
// BENCH(dynamic_query, 5)
//->ArgsProduct({{10'000'000},
//{10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
//{5},
////{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
//{DS_UNIFORM_FILL, DS_VISUAL_VAR}});
BENCH(dynamic_query, 5, BHLTree_t<5>, false)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 5},
                   //{1,  5,  10, 15, 20, 25, 30, 35, 40, 45, 50,
                   // 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {5},
                   //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
BENCH(dynamic_query, 5, LogTree_t<5>, true)
    ->ArgsProduct({{10'000'000},
                   {1, 2, 3, 5},
                   //{1,  5,  10, 15, 20, 25, 30, 35, 40, 45, 50,
                   // 55, 60, 65, 70, 75, 80, 85, 90, 95, 100},
                   {5},
                   //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                   {DS_UNIFORM_FILL, DS_VISUAL_VAR, DS_HOUSE_HOLD}});
