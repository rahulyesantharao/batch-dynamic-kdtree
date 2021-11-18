#ifndef BENCHMARK_UTILS_H
#define BENCHMARK_UTILS_H

#include "parlay/random.h"
#include <random>

// --- Taken from parlaylib ---
// Use this macro to avoid accidentally timing the destructors
// of the output produced by algorithms that return data
//
// The expression e is evaluated as if written in the context
// auto result_ = (e);
//
#define RUN_AND_CLEAR(e) \
  {                      \
    auto result_ = (e);  \
    state.PauseTiming(); \
  }                      \
  state.ResumeTiming();

// Define the benchmarks
#define BENCH(NAME, ...) \
  BENCHMARK_TEMPLATE(bench_##NAME, ##__VA_ARGS__)->UseRealTime()->Unit(benchmark::kMillisecond)

// --- Data loading ---
template <int dim>
void AddNoise(parlay::sequence<point<dim>>& points) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1e-6);

  for (size_t i = 0; i < points.size(); i++) {
    for (int d = 0; d < dim; d++) {
      points[i][d] += distribution(generator);
    }
  }
}

template <int dim, bool add_noise = false>
auto LoadFile(const char* filePath) {
  [[maybe_unused]] auto read_dim = readDimensionFromFile(filePath);
  assert(read_dim == dim);
  auto ret = readPointsFromFile<point<dim>>(filePath);
  if (add_noise) AddNoise<dim>(ret);
  return ret;
}

auto UniformSphere2D_1K() { return LoadFile<2>("datasets/2d-UniformInSphere-1k.pbbs"); }
auto UniformSphere2D_10K() { return LoadFile<2>("datasets/2d-UniformInSphere-10K.pbbs"); }
auto UniformSphere2D_1M() { return LoadFile<2>("datasets/2d-UniformInSphere-1M.pbbs"); }
auto UniformSphere2D_10M() { return LoadFile<2>("datasets/2d-UniformInSphere-10M.pbbs"); }
auto VisualVar5D_100K() { return LoadFile<5>("datasets/5D_VisualVar_100K.pbbs"); }
auto VisualVar5D_1M() { return LoadFile<5>("datasets/5D_VisualVar_1M.pbbs"); }

// Benchmark Datasets

auto UniformFill2D_10M() { return LoadFile<2>("datasets/2D_UniformFill_10M.pbbs"); }
auto UniformFill5D_10M() { return LoadFile<5>("datasets/5D_UniformFill_10M.pbbs"); }
auto UniformFill7D_10M() { return LoadFile<7>("datasets/7D_UniformFill_10M.pbbs"); }
auto VisualVar2D_10M() { return LoadFile<2, true>("datasets/2D_VisualVar_10M.pbbs"); }
auto VisualVar5D_10M() { return LoadFile<5, true>("datasets/5D_VisualVar_10M.pbbs"); }
auto VisualVar7D_10M() { return LoadFile<7, true>("datasets/7D_VisualVar_10M.pbbs"); }
auto GeoLife3D_24M() { return LoadFile<3, true>("datasets/3D_GeoLife_24M.pbbs"); }
auto HouseHold7D_2M() { return LoadFile<7, true>("datasets/7D_HouseHold_2M.pbbs"); }
auto HT10D_1M() { return LoadFile<10, true>("datasets/HT.pbbs"); }
auto CHEM16D_4M() { return LoadFile<16, true>("datasets/CHEM.pbbs"); }

enum DSType {
  DS_UNIFORM_FILL,    // 0
  DS_UNIFORM_SPHERE,  // 1
  DS_VISUAL_VAR,      // 2
  DS_GEO_LIFE,        // 3
  DS_HOUSE_HOLD,      // 4
  DS_HT,              // 5
  DS_CHEM             // 6
};
template <int dim>
auto BenchmarkDS(__attribute__((unused)) int size,
                 __attribute__((unused)) DSType ds_type,
                 bool shuffle = true) {
  std::stringstream ss;
  ss << "Invalid dim=" << dim << " to BenchmarkDS";
  throw std::runtime_error(ss.str());
}

template <>
auto BenchmarkDS<2>(int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<2>> ret;
  switch (ds_type) {
    case DS_UNIFORM_FILL: {
      ret = UniformFill2D_10M();
      break;
    }
    case DS_UNIFORM_SPHERE: {
      if (size == 1'000)
        ret = UniformSphere2D_1K();
      else if (size == 10'000)
        ret = UniformSphere2D_10K();
      else if (size == 1'000'000)
        ret = UniformSphere2D_1M();
      else if (size == 10'000'000)
        ret = UniformSphere2D_10M();
      else
        throw std::runtime_error("Invalid size to BenchmarkDS<2>(DS_UNIFORM_SPHERE)");
      break;
    }
    case DS_VISUAL_VAR: {
      ret = VisualVar2D_10M();
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<2>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

template <>
auto BenchmarkDS<3>(__attribute__((unused)) int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<3>> ret;
  switch (ds_type) {
    case DS_GEO_LIFE: {
      ret = GeoLife3D_24M();
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<3>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

template <>
auto BenchmarkDS<5>(int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<5>> ret;
  switch (ds_type) {
    case DS_UNIFORM_FILL: {
      ret = UniformFill5D_10M();
      break;
    }
    case DS_VISUAL_VAR: {
      if (size == 100'000)
        ret = VisualVar5D_100K();
      else if (size == 1'000'000)
        ret = VisualVar5D_1M();
      else if (size == 10'000'000)
        ret = VisualVar5D_10M();
      else
        throw std::runtime_error("Invalid size to BenchmarkDS<5>(DS_VISUAL_VAR)");
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<5>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

template <>
auto BenchmarkDS<7>(__attribute__((unused)) int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<7>> ret;
  switch (ds_type) {
    case DS_UNIFORM_FILL: {
      ret = UniformFill7D_10M();
      break;
    }
    case DS_VISUAL_VAR: {
      ret = VisualVar7D_10M();
      break;
    }
    case DS_HOUSE_HOLD: {
      ret = HouseHold7D_2M();
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<7>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

template <>
auto BenchmarkDS<10>(__attribute__((unused)) int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<10>> ret;
  switch (ds_type) {
    case DS_HT: {
      ret = HT10D_1M();
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<10>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

template <>
auto BenchmarkDS<16>(__attribute__((unused)) int size, DSType ds_type, bool shuffle) {
  parlay::sequence<point<16>> ret;
  switch (ds_type) {
    case DS_CHEM: {
      ret = CHEM16D_4M();
      break;
    }
    default:
      throw std::runtime_error("Invalid type to BenchmarkDS<16>");
  }

  if (shuffle)
    return random_shuffle(ret);
  else
    return ret;
}

// types
constexpr bool parallel = true;
constexpr bool coarsen = true;

template <int dim>
using COTree_t = CO_KdTree<dim, point<dim>, parallel, coarsen>;

template <int dim>
using BHLTree_t = BHL_KdTree<dim, point<dim>, parallel, coarsen>;

constexpr int NUM_TREES = 21;
constexpr int BUFFER_LOG2_SIZE = 10;
template <int dim>
using LogTree_t = LogTree<NUM_TREES, BUFFER_LOG2_SIZE, dim, point<dim>, parallel, coarsen>;

#endif  // BENCHMARK_UTILS_H
