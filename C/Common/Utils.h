#pragma once
#include "Benchmark.h"
#include <stdio.h>

unsigned char* Utils_ConvertRawImageSDR(RawImage* image, int* imageDataLength)
{
	*imageDataLength = image->width * image->height * 3;
	unsigned char* data = malloc(sizeof(unsigned char) * (*imageDataLength));
	int i = 0;
	//for (int y = 0; y != image.height; ++y)
	for (int y = image->height - 1; y != -1; --y)// invert Y
	{
		for (int x = 0; x != image->width; ++x)
		{
			Color color = Color_Mul2(image->colors[x + (y * image->width)], 255);
			data[i] = (unsigned char)MATH_Clamp(color.r, 0, 255);
			data[i + 1] = (unsigned char)MATH_Clamp(color.g, 0, 255);
			data[i + 2] = (unsigned char)MATH_Clamp(color.b, 0, 255);
			i += 3;
		}
	}
	return data;
}

void Utils_WriteFrame_PPM(FILE* stream, int width, int height, unsigned char* imageData, int imageDataLength)
{
	// PPM P6 binary header
	fprintf(stream, "P6\n%d %d\n255\n", width, height);

	// RGB pixel data
	fwrite(imageData, sizeof(unsigned char), imageDataLength, stream);
}

void Utils_SaveImageData_PPM(const char* filename, RawImage* image)
{
	// convert image to SDR data
	int imageDataLength;
	unsigned char* imageData = Utils_ConvertRawImageSDR(image, &imageDataLength);

	// save to file
	FILE* stream = fopen(filename, "wb");
	if (stream == NULL) return;

	// write frame
	Utils_WriteFrame_PPM(stream, image->width, image->height, imageData, imageDataLength);

	// finish
	fflush(stream);
	fclose(stream);
}

void Utils_SaveMovieData_PPM(const char* filename, RawMovie* movie)
{
	// save to file
	FILE* stream = fopen(filename, "wb");
	if (stream == NULL) return;

	for (int i = 0; i < movie->frameCount; ++i)
	{
		RawImage* image = movie->frames[i];

		// convert image to SDR data
		int imageDataLength;
		unsigned char* imageData = Utils_ConvertRawImageSDR(image, &imageDataLength);

		// write frame
		Utils_WriteFrame_PPM(stream, image->width, image->height, imageData, imageDataLength);
	}

	// finish
	fflush(stream);
	fclose(stream);
}