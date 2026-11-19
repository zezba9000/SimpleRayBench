#pragma once

#if defined(__APPLE__)
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif
#elif !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include <float.h>
#include <math.h>

#ifdef _WIN32
#include <Windows.h>
#include <threads.h>
typedef int ThreadResult;
#else
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef int BOOL;
#define FALSE 0
#define TRUE 1
typedef void* ThreadResult;
#endif

#if defined(NUM64)
// ===================================
// Math
// ===================================
static inline
double MATH_Clamp(double value, double min, double max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

static inline
double MATH_Sqrt(double value)
{
	return sqrt(value);
}

static inline
double MATH_Sin(double value)
{
	return sin(value);
}

static inline
double MATH_Cos(double value)
{
	return cos(value);
}

static inline
double MATH_Floor(double value)
{
	return floor(value);
}

static inline
double MATH_Pow(double value, double x)
{
	return pow(value, x);
}

static inline
double MATH_Max(double value, double x)
{
	return fmax(value, x);
}

static inline
double MATH_Min(double value, double x)
{
	return fmin(value, x);
}

static inline
double MATH_Abs(double value)
{
	return fabs(value);
}

static inline
double MATH_MaxValue()
{
	return DBL_MAX;
}
#elif defined(NUM32)
// ===================================
// MathF
// ===================================
static inline
float MATH_Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

static inline
float MATH_Sqrt(float value)
{
	return sqrtf(value);
}

static inline
float MATH_Sin(float value)
{
	return sinf(value);
}

static inline
float MATH_Cos(float value)
{
	return cosf(value);
}

static inline
float MATH_Floor(float value)
{
	return floorf(value);
}

static inline
float MATH_Pow(float value, float x)
{
	return powf(value, x);
}

static inline
float MATH_Max(float value, float x)
{
	return fmaxf(value, x);
}

static inline
float MATH_Min(float value, float x)
{
	return fminf(value, x);
}

static inline
float MATH_Abs(float value)
{
	return fabsf(value);
}

static inline
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
#ifdef _WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
#else
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}