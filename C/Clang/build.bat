mkdir bin
clang -std=c11 -O3 -flto -fno-math-errno -fuse-ld=lld Clang.c -o ./bin/Benchmark.exe