#ifndef KDTREE_SHARED_BOX_H
#define KDTREE_SHARED_BOX_H

// TODO: refactor this file into a class after [point] has a move constructor, copy constructor

enum BoxComparison { BOX_INCLUDE = 0, BOX_OVERLAP, BOX_EXCLUDE };

/*!
 * <Serial> Check whether [item] is included in the given box.
 * @param pMin1 the minimum point of the box
 * @param pMax1 the maximum point of the box
 * @param item the item to check against the box
 * @return whether [item] is in the box
 */
template <int dim, class objT>
inline bool itemInBox(const point<dim> &pMin1, const point<dim> &pMax1, const objT *item) {
  for (int i = 0; i < dim; ++i) {
    if (pMax1.coordinate(i) < item->coordinate(i) || pMin1.coordinate(i) > item->coordinate(i))
      return false;
  }
  return true;
}
/*!
 * <Serial> Compare two boxes defined by their min/max points.
 * @param pMin1 the minimum point of box 1
 * @param pMax1 the maximum point of box 1
 * @param pMin2 the minimum point of box 2
 * @param pMax2 the maximum point of box 2
 * @return [BoxComparison] of box1 to box2
 */
template <int dim>
inline BoxComparison boxCompare(const point<dim> &pMin1,
                                const point<dim> &pMax1,
                                const point<dim> &pMin2,
                                const point<dim> &pMax2) {
  bool exclude = false;
  bool include = true;  // 1 include 2
  for (int i = 0; i < dim; ++i) {
    if (pMax1.coordinate(i) < pMin2.coordinate(i) || pMin1.coordinate(i) > pMax2.coordinate(i))
      exclude = true;
    if (pMax1.coordinate(i) < pMax2.coordinate(i) || pMin1.coordinate(i) > pMin2.coordinate(i))
      include = false;
  }
  if (exclude)
    return BOX_EXCLUDE;
  else if (include)
    return BOX_INCLUDE;
  else
    return BOX_OVERLAP;
}

// Assumes the nodes have up-to-date bounding boxes!
// Taken from: https://github.com/scipy/scipy/blob/v1.6.3/scipy/spatial/kdtree.py#L153-L165
template <int dim>
double BoundingBoxDistance(const point<dim> &pMin1,
                           const point<dim> &pMax1,
                           const point<dim> &pMin2,
                           const point<dim> &pMax2) {
  double dist = 0;
  for (int i = 0; i < dim; ++i) {
    // compute the shortest distance in this dimension
    double dim_val = 0;
    dim_val = std::max(dim_val, pMin1.coordinate(i) - pMax2.coordinate(i));
    dim_val = std::max(dim_val, pMin2.coordinate(i) - pMax1.coordinate(i));
    dist += dim_val * dim_val;
  }
  return std::sqrt(dist);
}

/*!
 * <Serial> (Re)compute bounding box for [items] under this node: store in [pMin], [pMax].
 */
template <int dim, class objT>
inline void boundingBoxSerial(point<dim> &pMin,
                              point<dim> &pMax,
                              const parlay::slice<objT *, objT *> &items) {
  typedef point<dim> pointT;
  pMin = pointT(items[0].coordinate());
  pMax = pointT(items[0].coordinate());
  for (size_t i = 0; i < items.size(); ++i) {
    pMin.minCoords(items[i].coordinate());
    pMax.maxCoords(items[i].coordinate());
  }
}

/*!
 * <Parallel> (Re)compute bounding box for [items] under this node: store in [pMin], [pMax].
 */
template <int dim, class objT>
inline void boundingBoxParallel(point<dim> &pMin,
                                point<dim> &pMax,
                                const parlay::slice<objT *, objT *> &items) {
  typedef point<dim> pointT;
  auto P = parlay::num_workers() * 8;
  auto blockSize = (items.size() + P - 1) / P;
  pointT localMin[P];
  pointT localMax[P];
  for (size_t i = 0; i < P; ++i) {
    localMin[i] = pointT(items[0].coordinate());
    localMax[i] = pointT(items[0].coordinate());
  }
  parlay::parallel_for(0, P, [&](size_t p) {
    auto s = p * blockSize;
    auto e = min((p + 1) * blockSize, items.size());
    for (auto j = s; j < e; ++j) {
      localMin[p].minCoords(items[j].coordinate());
      localMax[p].maxCoords(items[j].coordinate());
    }
  });
  pMin = pointT(items[0]->coordinate());
  pMax = pointT(items[0]->coordinate());
  for (size_t p = 0; p < P; ++p) {
    pMin.minCoords(localMin[p].x);
    pMax.maxCoords(localMax[p].x);
  }
}

template <int dim>
class Box {
  typedef point<dim> pointT;

  pointT pMin, pMax;

 public:
  BoxComparison compare(const Box &other) {
    return boxCompare<dim>(pMin, pMax, other.pMin, other.pMax);
  }

  template <class objT>
  bool contains(const objT *o) {
    return itemInBox<dim, objT>(pMin, pMax, o);
  }
};

#endif  // KDTREE_SHARED_BOX_H
