# Parallel, Batch-Dynamic kd-Tree
## About
This repository contains code for our paper: Parallel Batch-Dynamic kd-Trees. It contains an implementation of a logarithmically-structured parallel batch-dynamic kd-tree as well as several baseline implementations of parallel batch-dynamic kd-trees.

## Development
We use [ParlayLib](https://github.com/cmuparlay/parlaylib), a wonderful library developed at CMU for multi-core parallel programming. We also use [xxHash](https://github.com/Cyan4973/xxHash) for a 64-bit hash function. These modules are included in the project as submodules, initialize them before building the project:
```
git submodule init
git submodule update
```

We use CMake as the build system. To build the project:
```
mkdir build
cd build
cmake ..
make -j
```
See the top level [CMakeLists](https://github.com/rahulyesantharao/batch-dynamic-kdtree/blob/main/CMakeLists.txt#L41) for various build options that can be included with the `cmake` command above.

After the build completes, you will find executables in the nested directories of `build/`. Comprehensive tests can be found under `build/test/` for all the implemented data structures. Benchmarks are written using [Google Benchmark](https://github.com/google/benchmark) and can be found under `build/benchmark/`. They can be run using GoogleBenchmark syntax; for example, to run the construction benchmark over the BDL-tree with a 10M, 2D dataset, use
```
./bench_construction --benchmark_filter="bench_construction<2, LogTree_t<2>>/10000000/100/0/real_time"
```

You can view all the different benchmarks under a given executable by passing the flag `--benchmark_list_tests`. Example datasets can be found [here](https://github.com/rahulyesantharao/batch-dynamic-kdtree/tree/main/test/resources).

## Support 
This code is still in a research-phase. Please contact the developers or paper authors if you encounter any problems.
