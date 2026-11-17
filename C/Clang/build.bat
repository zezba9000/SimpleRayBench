mkdir bin
clang -std=c11 -O3 -flto -DWIN32_CLANG Clang.c -o ./bin/Benchmark