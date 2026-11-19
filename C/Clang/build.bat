mkdir bin
clang -std=c11 -O3 -flto -fuse-ld=lld Clang.c -o ./bin/Benchmark.exe