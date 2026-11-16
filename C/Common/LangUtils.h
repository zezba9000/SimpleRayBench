#pragma once
#include <math.h>
#include <thread>

#if defined(WINDOWS)
#include <Windows.h>
#else
#include <cfloat>
#include <limits>
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

#if defined(NUM64)
// ===================================
// Math
// ===================================
inline
double Math_Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline
double Math_Sqrt(double value)
{
	return sqrt(value);
}

inline
double Math_Sin(double value)
{
	return sin(value);
}

inline
double Math_Cos(double value)
{
	return cos(value);
}

inline
double Math_Floor(double value)
{
	return floor(value);
}

inline
double Math_Pow(double value, double x)
{
	return pow(value, x);
}

inline
double Math_Max(double value, double x)
{
	return fmax(value, x);
}

inline
double Math_Min(double value, double x)
{
	return fmin(value, x);
}

inline
double Math_Abs(double value)
{
	return abs(value);
}

inline
double Math_MaxValue()
{
	return DBL_MAX;
}
#elif defined(NUM32)
// ===================================
// MathF
// ===================================
inline
float Math_Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline
float Math_Sqrt(float value)
{
	return sqrt(value);
}

inline
float Math_Sin(float value)
{
	return sin(value);
}

inline
float Math_Cos(float value)
{
	return cos(value);
}

inline
float Math_Floor(float value)
{
	return floor(value);
}

inline
float Math_Pow(float value, float x)
{
	return pow(value, x);
}

inline
float Math_Max(float value, float x)
{
	return fmax(value, x);
}

inline
float Math_Min(float value, float x)
{
	return fmin(value, x);
}

inline
float Math_Abs(float value)
{
	return abs(value);
}

inline
float Math_MaxValue()
{
	return FLT_MAX;
}
#endif

// ===================================
// Math
// ===================================
int Environment_ProcessorCount()
{
#ifdef WINDOWS
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
#else
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}