mkdir -p bin
clang++ -std=c++11 -O3 -flto -pthread ./Clang.cpp -o ./bin/Benchmark