#include "../../Common/Benchmark_Main.h"
#include <stdio.h>

int main()
{
	RawMovie* movie = Benchmark_Main_Run();
	Benchmark_Main_Save(movie);
	return 0;
}