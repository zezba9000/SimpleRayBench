#define WINDOWS
#define inline __inline
#include "../../Common/Benchmark.h"
#include "../../Common/Utils.h"

#include <stdio.h>

int main()
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

	Utils_SaveMovieData_PPM("image.ppm", movie);
	Benchmark_Dispose();

	return 0;
}