#pragma once
#include "Benchmark.h"
#include "Utils.h"

#include <stdio.h>

RawMovie* Benchmark_Main_Run()
{
	const int size = 512;// image size
	Benchmark_Init(size, size, 200);

#ifdef _WIN32
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
#else
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
#endif

	RawMovie* movie = Benchmark_Render(FALSE);// run benchmark

#ifdef _WIN32
	QueryPerformanceCounter(&end);
	double sec = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
#else
	clock_gettime(CLOCK_MONOTONIC, &end);
	double sec = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif
	printf("Render Time: %fs\n", sec);
	return movie;
}

void Benchmark_Main_Save(RawMovie* movie)
{
	Utils_SaveMovieData_PPM("image.ppm", movie);
	Benchmark_Dispose();
}