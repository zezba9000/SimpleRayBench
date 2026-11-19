mkdir -p bin
gcc -std=c11 -O3 -flto -pthread ./GCC.c -o ./bin/Benchmark -lm