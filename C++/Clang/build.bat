if not exist bin mkdir bin
clang++ -std=c++14 -O3 -flto -fuse-ld=lld Clang.cpp -o ./bin/Benchmark.exe