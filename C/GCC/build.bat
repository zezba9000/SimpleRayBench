mkdir bin
gcc -std=c11 -O3 -flto -DWIN32_GCC GCC.c -o ./bin/Benchmark