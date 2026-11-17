using System;
using System.Diagnostics;

namespace SimpleRayBench
{
	internal class Program
	{
		static void Main(string[] args)
		{
			var movie = Benchmark_Main.Run();
			Benchmark_Main.Save(movie);
		}
	}
}