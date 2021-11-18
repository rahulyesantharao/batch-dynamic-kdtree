#include <iostream>
#include <algorithm>

#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include "parlay/primitives.h"

#include "common/geometry.h"
#include "common/geometryIO.h"

#include "../include/kdtree/shared/utils.h"
#include "../include/kdtree/shared/macro.h"

void testScan() {
  parlay::sequence<int> p;
  for (int i = 0; i < 10; i++)
    p.push_back(i);

  auto [res, sum] = parlay::scan(p);

  for (const auto& r : res)
    std::cout << r << " ";
  std::cout << "\nsum = " << sum << std::endl;

  // another test
  auto res1 = parlay::scan_inclusive(p);
  for (const auto& r : res1)
    std::cout << r << " ";
}

void testLoop(int argc, const char** argv) {
  if (argc != 2) throw std::runtime_error("need to pass file name!");
  const char* test_file = argv[1];

  constexpr int dim = 2;
  int check_dim = readDimensionFromFile(test_file);
  if (check_dim != dim) throw std::runtime_error("Invalid input file!");
  auto P = readPointsFromFile<point<dim>>(test_file);
  auto P_slice = parlay::slice(P.begin(), P.end());

  parlay::sequence<point<dim>> Q(P.size());
  auto Q_slice = parlay::slice(Q.begin(), Q.end());

  timer t;
  parlay::parallel_for(
      0, P_slice.size(), [Q_slice, P_slice](size_t i) { Q_slice[i] = P_slice[i]; });
  std::cout << t.get_next() << std::endl;
}

static parlay::sequence<point<2>> PARLAY_2D_TEST_POINTS(size_t num_points) {
  constexpr int dim = 2;
  std::vector<point<dim>> point_vec(num_points);
  for (size_t i = 0; i < num_points; i++)
    point_vec[i] = point<dim>({(double)i, (double)i});
  parlay::sequence<point<dim>> points(point_vec.begin(), point_vec.end());
  return points;
}

void testSpatial(int argc, const char** argv) {
  size_t size;
  if (argc == 1)
    size = 8;
  else if (argc == 2)
    size = std::stoul(argv[1]);
  else
    throw std::runtime_error("invalid input");

  auto points = PARLAY_2D_TEST_POINTS(size);
  parlay::sequence<bool> flags(points.size());
  size_t split_pt;
  auto median = parallelSpatialPartition(
      points.cut(0, points.size()), flags.cut(0, flags.size()), 0, split_pt);

  std::cout << "(median, split_pt) = (" << median << ", " << split_pt << ")" << std::endl;
}

int main(int argc, const char** argv) { print_config(); }
