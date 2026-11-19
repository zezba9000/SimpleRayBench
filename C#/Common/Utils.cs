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
			ConvertRawImageSDR(image, data);
			return data;
		}

		private static void ConvertRawImageSDR(RawImage image, byte[] data)
		{
			int i = 0;
			//for (int y = 0; y != image.height; ++y)// normal Y
			for (int y = image.height - 1; y != -1; --y)// invert Y
			{
				int rowOffset = y * image.width;
				for (int x = 0; x != image.width; ++x)
				{
					var color = image.colors[x + rowOffset] * 255;
					data[i] = (byte)Math.Clamp(color.r, 0, 255);
					data[i + 1] = (byte)Math.Clamp(color.g, 0, 255);
					data[i + 2] = (byte)Math.Clamp(color.b, 0, 255);
					i += 3;
				}
			}
		}

		private static void WriteFrame_PPM(FileStream stream, int width, int height, byte[] imageData, int imageDataLength)
		{
			// PPM P6 binary header
			byte[] header = Encoding.ASCII.GetBytes($"P6\n{width} {height}\n255\n");
			stream.Write(header, 0, header.Length);

			// RGB pixel data
			stream.Write(imageData, 0, imageDataLength);
		}


		public static void SaveImageData_PPM(string filename, RawImage image)
		{
			// convert image to SDR data
			byte[] imageData = ConvertRawImageSDR(image);

			// save to file
			using (var stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Read))
			{
				// write frame
				WriteFrame_PPM(stream, image.width, image.height, imageData, imageData.Length);

				// finish
				stream.Flush();
			}
		}

		public static void SaveMovieData_PPM(string filename, RawMovie movie)
		{
			// save to file
			using (var stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Read))
			{
				var imageData = Array.Empty<byte>();
				foreach (var image in movie.frames)
				{
					int imageDataLength = image.width * image.height * 3;
					if (imageData.Length < imageDataLength) imageData = new byte[imageDataLength];
					ConvertRawImageSDR(image, imageData);

					// write frame
					WriteFrame_PPM(stream, image.width, image.height, imageData, imageDataLength);
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