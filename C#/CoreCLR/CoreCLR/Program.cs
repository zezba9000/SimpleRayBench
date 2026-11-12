using System;
using System.Diagnostics;

namespace SimpleRayBench
{
	internal class Program
	{
		static void Main(string[] args)
		{
			const int size = 512;
			Benchmark.Init(size, size, 1);

			var watch = new Stopwatch();
			watch.Start();
			var movie = Benchmark.Render(false);
			watch.Stop();
			Console.WriteLine($"Render Time: {Math.Round(watch.Elapsed.TotalSeconds, 2)}s");

			Utils.SaveMovieData_PPM("image.ppm", movie);
			Benchmark.Dispose();
		}
	}
}