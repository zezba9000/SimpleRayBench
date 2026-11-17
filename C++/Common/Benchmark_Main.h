#pragma once
#include "Benchmark.h"
#include "Utils.h"
#include <iostream>
#include <ctime>
#include <chrono>

namespace SimpleRayBench
{
	class Benchmark_Main
	{
	public:
		static RawMovie* Run()
		{
			const int size = 512;// image size
			Benchmark::Init(size, size, 200);

			auto start = std::chrono::high_resolution_clock::now();
			auto movie = Benchmark::Render(false);// run benchmark
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> sec = end - start;
			std::cout << "Render Time: " << sec.count() << "s" << std::endl;
			return movie;
		}

		static void Save(RawMovie* movie)
		{
			Utils::SaveMovieData_PPM("image.ppm", movie);
			Benchmark::Dispose();
		}
	};
}