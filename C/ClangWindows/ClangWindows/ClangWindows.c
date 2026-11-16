#include <iostream>

#define WINDOWS
#include "../../Common/Benchmark.h"
#include "../../Common/Utils.h"

#include <ctime>
#include <chrono>

using namespace SimpleRayBench;

int main()
{
	const int size = 512;// image size
	Benchmark::Init(size, size, 200);

	auto start = std::chrono::high_resolution_clock::now();
	auto movie = Benchmark::Render(false);// run benchmark
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> sec = end - start;
	std::cout << "Render Time: " << sec.count() << "s" << std::endl;

	Utils::SaveMovieData_PPM("image.ppm", movie);
	Benchmark::Dispose();

	return 0;
}