#pragma once
#include "Benchmark.h"
#include <fstream>

namespace SimpleRayBench
{
	class Utils
	{
	public:
		static char* ConvertRawImageSDR(RawImage* image, int* imageDataLength)
		{
			*imageDataLength = image->width * image->height * 3;
			auto data = new char[*imageDataLength];
			int i = 0;
			//for (int y = 0; y != image.height; ++y)
			for (int y = image->height - 1; y != -1; --y)// invert Y
			{
				for (int x = 0; x != image->width; ++x)
				{
					auto color = image->colors[x + (y * image->width)] * 255;
					data[i] = (char)Math::Clamp(color.r, 0, 255);
					data[i + 1] = (char)Math::Clamp(color.g, 0, 255);
					data[i + 2] = (char)Math::Clamp(color.b, 0, 255);
					i += 3;
				}
			}
			return data;
		}

		static void WriteFrame_PPM(std::ofstream& stream, int width, int height, char* imageData, int imageDataLength)
		{
			// PPM P6 binary header
			stream << "P6\n" << width << " " << height << "\n255\n";

			// RGB pixel data
			stream.write(imageData, imageDataLength);
		}

		static void SaveImageData_PPM(const char* filename, RawImage* image)
		{
			// convert image to SDR data
			int imageDataLength;
			char* imageData = ConvertRawImageSDR(image, &imageDataLength);

			// save to file
			std::ofstream stream;
			stream.open(filename, std::ios::binary | std::ios::out);

			// write frame
			WriteFrame_PPM(stream, image->width, image->height, imageData, imageDataLength);

			// finish
			stream.flush();
			stream.close();
		}

		static void SaveMovieData_PPM(const char* filename, RawMovie* movie)
		{
			// save to file
			std::ofstream stream;
			stream.open(filename, std::ios::binary | std::ios::out);

			for (int i = 0; i < movie->frameCount; ++i)
			{
				RawImage* image = movie->frames[i];

				// convert image to SDR data
				int imageDataLength;
				char* imageData = ConvertRawImageSDR(image, &imageDataLength);

				// write frame
				WriteFrame_PPM(stream, image->width, image->height, imageData, imageDataLength);
			}

			// finish
			stream.flush();
			stream.close();
		}
	};
}