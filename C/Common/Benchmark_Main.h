#pragma once
#include "Benchmark.h"
#include "Utils.h"

#include <stdio.h>

RawMovie* Benchmark_Main_Run()
{
	const int size = 512;// image size
	Benchmark_Init(size, size, 200);

	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	RawMovie* movie = Benchmark_Render(FALSE);// run benchmark

	QueryPerformanceCounter(&end);
	double sec = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
	printf("Render Time: %fs\n", sec);
	return movie;
}

void Benchmark_Main_Save(RawMovie* movie)
{
	Utils_SaveMovieData_PPM("image.ppm", movie);
	Benchmark_Dispose();
}