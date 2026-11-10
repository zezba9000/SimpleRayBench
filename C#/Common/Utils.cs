using System;
using System.IO;
using System.Text;

namespace SimpleRayBench
{
	public static class Utils
	{
		public static byte[] ConvertRawImageSDR(RawImage image)
		{
			var data = new byte[image.width * image.height * 3];
			int i = 0;
			//for (int y = 0; y != image.height; ++y)// normal Y
			for (int y = image.height - 1; y != -1; --y)// invert Y
			{
				for (int x = 0; x != image.width; ++x)
				{
					var color = image.colors[x, y] * 255;
					data[i] = (byte)Math.Clamp(color.r, 0, 255);
					data[i + 1] = (byte)Math.Clamp(color.g, 0, 255);
					data[i + 2] = (byte)Math.Clamp(color.b, 0, 255);
					i += 3;
				}
			}
			return data;
		}

		private static void WriteFrame_PPM(FileStream stream, int width, int height, byte[] imageData)
		{
			// PPM P6 binary header
			byte[] header = Encoding.ASCII.GetBytes($"P6\n{width} {height}\n255\n");
			stream.Write(header, 0, header.Length);

			// RGB pixel data
			stream.Write(imageData, 0, imageData.Length);
		}


		public static void SaveImageData_PPM(string filename, RawImage image)
		{
			// convert image to SDR data
			byte[] imageData = ConvertRawImageSDR(image);

			// save to file
			using (var stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Read))
			{
				// write frame
				WriteFrame_PPM(stream, image.width, image.height, imageData);

				// finish
				stream.Flush();
			}
		}

		public static void SaveMovieData_PPM(string filename, RawMovie movie)
		{
			// save to file
			using (var stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Read))
			{
				foreach (var image in movie.frames)
				{
					// convert image to SDR data
					byte[] imageData = ConvertRawImageSDR(image);

					// write frame
					WriteFrame_PPM(stream, image.width, image.height, imageData);
				}

				// finish
				stream.Flush();
			}
		}

		public static void SeedRandomValues(int[] values)
		{
			var random = new Random();
			for (int i = 0; i != values.Length; ++i)
			{
				values[i] = random.Next();
			}
		}

		public static void PrintRandomValues(int count)
		{
			var random = new Random();
			for (int i = 0; i != count; ++i)
			{
				Console.Write(random.NextSingle().ToString() + ",");
				if (i % 10 == 9) Console.WriteLine();
			}
		}
	}
}