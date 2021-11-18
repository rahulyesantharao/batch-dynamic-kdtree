# Benchmarks

## Disable CPU frequency scaling
See [here](https://github.com/google/benchmark#disabling-cpu-frequency-scaling).

tldr:
```
sudo cpupower frequency-set --governor performance
./mybench
sudo cpupower frequency-set --governor powersave
```
