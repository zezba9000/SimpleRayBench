using System;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace SimpleRayBench
{
	public static class Benchmark_Main
	{
		public static RawMovie Run()
		{
			const int size = 512;
			Benchmark.Init(size, size, 200);

			var watch = new Stopwatch();
			watch.Start();
			var movie = Benchmark.Render(false);
			watch.Stop();
			Console.WriteLine($"Render Time: {Math.Round(watch.Elapsed.TotalSeconds, 2)}s");
			return movie;
		}

		public static void Save(RawMovie movie)
		{
			Utils.SaveMovieData_PPM("image.ppm", movie);
			Benchmark.Dispose();
		}
	}
}