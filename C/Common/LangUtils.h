#pragma once
#include <math.h>
#include <threads.h>

#if defined(WINDOWS)
#include <Windows.h>
#include <float.h>
#include <stdio.h>
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
double MATH_Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline
double MATH_Sqrt(double value)
{
	return sqrt(value);
}

inline
double MATH_Sin(double value)
{
	return sin(value);
}

inline
double MATH_Cos(double value)
{
	return cos(value);
}

inline
double MATH_Floor(double value)
{
	return floor(value);
}

inline
double MATH_Pow(double value, double x)
{
	return pow(value, x);
}

inline
double MATH_Max(double value, double x)
{
	return fmax(value, x);
}

inline
double MATH_Min(double value, double x)
{
	return fmin(value, x);
}

inline
double MATH_Abs(double value)
{
	return fabs(value);
}

inline
double MATH_MaxValue()
{
	return DBL_MAX;
}
#elif defined(NUM32)
// ===================================
// MathF
// ===================================
inline
float MATH_Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline
float MATH_Sqrt(float value)
{
	return sqrtf(value);
}

inline
float MATH_Sin(float value)
{
	return sinf(value);
}

inline
float MATH_Cos(float value)
{
	return cosf(value);
}

inline
float MATH_Floor(float value)
{
	return floorf(value);
}

inline
float MATH_Pow(float value, float x)
{
	return powf(value, x);
}

inline
float MATH_Max(float value, float x)
{
	return fmaxf(value, x);
}

inline
float MATH_Min(float value, float x)
{
	return fminf(value, x);
}

inline
float MATH_Abs(float value)
{
	return fabsf(value);
}

inline
float MATH_MaxValue()
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