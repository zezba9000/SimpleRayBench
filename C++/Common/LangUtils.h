#pragma once
#include <cmath>
#include <thread>

#if defined(WINDOWS) || defined(_WIN32)
#include <Windows.h>
#include <cfloat>
#else
#include <cfloat>
#include <limits>
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

class Math
{
public:
	inline
	static double Clamp(double value, double min, double max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	inline
	static double Sqrt(double value)
	{
		return std::sqrt(value);
	}

	inline
	static double Sin(double value)
	{
		return std::sin(value);
	}

	inline
	static double Cos(double value)
	{
		return std::cos(value);
	}

	inline
	static double Floor(double value)
	{
		return std::floor(value);
	}

	inline
	static double Pow(double value, double x)
	{
		return std::pow(value, x);
	}

	inline
	static double Pow(double value, int x)
	{
		#if defined(__GNUC__) || defined(__clang__)
		return __builtin_powi(value, x);
		#else
		return std::pow(value, x);
		#endif
	}

	inline
	static double Max(double value, double x)
	{
		return (value > x) ? value : x;
	}

	inline
	static double Min(double value, double x)
	{
		return (value < x) ? value : x;
	}

	inline
	static double Abs(double value)
	{
		return std::abs(value);
	}

	inline
	static double MaxValue()
	{
		return DBL_MAX;
	}
};

class MathF
{
public:
	inline
	static float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	inline
	static float Sqrt(float value)
	{
		return std::sqrt(value);
	}

	inline
	static float Sin(float value)
	{
		return std::sin(value);
	}

	inline
	static float Cos(float value)
	{
		return std::cos(value);
	}

	inline
	static float Floor(float value)
	{
		return std::floor(value);
	}

	inline
	static float Pow(float value, float x)
	{
		return powf(value, x);
	}

	inline
	static float Pow(float value, int x)
	{
		#if defined(__GNUC__) || defined(__clang__)
		return __builtin_powif(value, x);
		#else
		return std::pow(value, x);
		#endif
	}

	inline
	static float Max(float value, float x)
	{
		return (value > x) ? value : x;
	}

	inline
	static float Min(float value, float x)
	{
		return (value < x) ? value : x;
	}

	inline
	static float Abs(float value)
	{
		return std::abs(value);
	}

	inline
	static float MaxValue()
	{
		return FLT_MAX;
	}
};

class Environment
{
public:
	static int ProcessorCount()
	{
		return std::thread::hardware_concurrency();
	}
};