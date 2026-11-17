#include <iostream>

#define WINDOWS
#include "../../Common/Benchmark_Main.h"
using namespace SimpleRayBench;

int main()
{
	auto movie = Benchmark_Main::Run();
	Benchmark_Main::Save(movie);
	return 0;
}