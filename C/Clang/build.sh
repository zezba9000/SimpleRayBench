mkdir -p bin
clang -std=c11 -O3 -flto -pthread ./Clang.c -o ./bin/Benchmark -lm