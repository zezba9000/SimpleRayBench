#pragma once

#define NUM32
//#define NUM64
//#define DISABLE_FOG

#if defined(NUM32)
#define Num float
#elif defined(NUM64)
#define Num double
#endif

#include "LangUtils.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct RandomUtil
{
	Num offsetX, offsetY;
}RandomUtil;

static inline
Num RandomUtil_NextValue(RandomUtil* this, Num u, Num v)
{
	#define randoValueX 12.9898f
	#define randoValueY 78.233f
	#define randoValueZ 43758.543123f

	u += this->offsetX;
	v += this->offsetY;
	this->offsetX += .5f;
	this->offsetY += .75f;

	Num result = (u * randoValueX) + (v * randoValueY);
	result = MATH_Sin(result) * randoValueZ;
	return result - MATH_Floor(result);
}

typedef struct Color
{
	Num r, g, b;
}Color;

Color Color_Constructor()
{
	Color this;
	this.r = 0;
	this.g = 0;
	this.b = 0;
	return this;
}

Color Color_Constructor2(Num r, Num g, Num b)
{
	Color this;
	this.r = r;
	this.g = g;
	this.b = b;
	return this;
}

static inline
Color Color_Add(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b);
}

static inline
Color Color_Sub(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b);
}

static inline
Color Color_Mul(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r * p2.r, p1.g * p2.g, p1.b * p2.b);
}

static inline
Color Color_Mul2(Color this, Num p2)
{
	return Color_Constructor2(this.r * p2, this.g * p2, this.b * p2);
}

static inline
Color Color_Div(Color this, const Color p1, Num p2)
{
	return Color_Constructor2(p1.r / p2, p1.g / p2, p1.b / p2);
}

static inline
Color Color_Lerp(Color p1, Color p2, Num t)
{
	return Color_Add(p1, Color_Mul2(Color_Sub(p2, p1), t));
}

static inline
Color Color_LerpClamp(Color p1, Color p2, Num t)
{
	return Color_Add(p1, Color_Mul2(Color_Sub(p2, p1), MATH_Clamp(t, 0, 1)));
}

typedef struct Vec3
{
	Num x, y, z;
}Vec3;

Vec3 Vec3_Constructor()
{
	Vec3 this;
	this.x = 0;
	this.y = 0;
	this.z = 0;
	return this;
}

Vec3 Vec3_Constructor2(Num x, Num y, Num z)
{
	Vec3 this;
	this.x = x;
	this.y = y;
	this.z = z;
	return this;
}

static inline
Vec3 Vec3_Add(Vec3 this, const Vec3 p2)
{
	return Vec3_Constructor2(this.x + p2.x, this.y + p2.y, this.z + p2.z);
}

static inline
Vec3 Vec3_Sub(Vec3 this, const Vec3 p2)
{
	return Vec3_Constructor2(this.x - p2.x, this.y - p2.y, this.z - p2.z);
}

static inline
Vec3 Vec3_Sub2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x - p2, p1.y - p2, p1.z - p2);
}

static inline
Vec3 Vec3_Neg(Vec3 v)
{
	return Vec3_Constructor2(-v.x, -v.y, -v.z);
}

static inline
Vec3 Vec3_Mul(Vec3 p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}

static inline
Vec3 Vec3_Mul2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x * p2, p1.y * p2, p1.z * p2);
}

static inline
Vec3 Vec3_Mul3(Num p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1 * p2.x, p1 * p2.y, p1 * p2.z);
}

static inline
Vec3 Vec3_Div(Vec3 p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
}

static inline
Vec3 Vec3_Div2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x / p2, p1.y / p2, p1.z / p2);
}

static inline
Num Vec3_Dot(Vec3 this)
{
	return (this.x * this.x) + (this.y * this.y) + (this.z * this.z);
}

static inline
Num Vec3_Dot2(Vec3 this, Vec3 vec)
{
	return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
}

static inline
Num Vec3_Length(Vec3 this)
{
	return MATH_Sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
}

static inline
Vec3 Vec3_Normalize(Vec3 this)
{
	return Vec3_Div2(this, MATH_Sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z)));
}

static inline
Vec3 Vec3_Reflect(Vec3 this, Vec3 planeNormal)
{
	return Vec3_Sub(this, Vec3_Mul2(planeNormal, Vec3_Dot2(this, planeNormal) * 2));
}

static inline
Vec3 Vec3_Cross(Vec3 this, Vec3 vector)
{
	return Vec3_Constructor2(((this.y * vector.z) - (this.z * vector.y)), ((this.z * vector.x) - (this.x * vector.z)), ((this.x * vector.y) - (this.y * vector.x)));
}

static inline
Num Vec3_FresnelSchlickZero(Vec3 rayDirection, Vec3 normal)
{
	Num value = 1 - MATH_Max(Vec3_Dot2(Vec3_Neg(rayDirection), normal), 0);
	Num squared = value * value;
	return squared * squared * value;
}

typedef struct Mat3
{
	Vec3 x, y, z;
}Mat3;

Vec3 Mat3_Transform(Mat3 this, Vec3 v)
{
	return Vec3_Add(Vec3_Add(Vec3_Mul2(this.x, v.x), Vec3_Mul2(this.y, v.y)), Vec3_Mul2(this.z, v.z));
}

typedef struct HitInfo
{
	Vec3 point, normal;
}HitInfo;

HitInfo HitInfo_Constructor()
{
	HitInfo this;
	this.point = Vec3_Constructor();
	this.normal = Vec3_Constructor();
	return this;
}

typedef struct Ray
{
	Vec3 origin, direction;
}Ray;

Ray Ray_Constructor()
{
	Ray this;
	this.origin = Vec3_Constructor();
	this.direction = Vec3_Constructor();
	return this;
}

Ray Ray_Constructor2(Vec3 origin, Vec3 direction)
{
	Ray this;
	this.origin = origin;
	this.direction = direction;
	return this;
}

static inline
BOOL Ray_HitSphereNumerator(const Ray* this, Vec3 sphereCenter, Num sphereRadius, Num a, Num* numerator)
{
	Vec3 d = Vec3_Sub(this->origin, sphereCenter);
	Num b = Vec3_Dot2(d, this->direction);
	if (b >= 0) return FALSE;

	Num c = Vec3_Dot(d) - sphereRadius * sphereRadius;
	Num disc = b * b - a * c;
	if (disc < 0) return FALSE;

	*numerator = -b - MATH_Sqrt(disc);
	return TRUE;
}

BOOL Ray_HitSphere(Ray* this, Vec3 sphereCenter, Num sphereRadius, HitInfo* info)
{
	Num a = Vec3_Dot(this->direction);
	Num numerator;
	if (!Ray_HitSphereNumerator(this, sphereCenter, sphereRadius, a, &numerator))
	{
		*info = HitInfo_Constructor();
		return FALSE;
	}

	Num t = numerator * (1 / a);
	info->point = Vec3_Add(this->origin, Vec3_Mul3(t, this->direction));
	info->normal = Vec3_Normalize(Vec3_Sub(info->point, sphereCenter));

	return TRUE;
}

typedef struct Material
{
	Color color;
	Num roughness;
	Num reflectiveness, metalic;
}Material;

Material Material_Constructor()
{
	Material this = {0};
	return this;
}

Material Material_Constructor2(Color color, Num roughness, Num reflectiveness, Num metalic)
{
	Material this;
	this.color = color;
	this.roughness = roughness;
	this.reflectiveness = reflectiveness;
	this.metalic = metalic;
	return this;
}

typedef enum ObjType
{
	ObjType_Plane,
	ObjType_Sphere
}ObjType;

typedef struct Obj
{
	ObjType type;
	Material material;
}Obj;

Obj Obj_Constructor(ObjType type)
{
	Obj this;
	this.material = Material_Constructor();
	this.type = type;
	return this;
}

typedef struct Sphere
{
	Obj base;
	Vec3 center;
	Num radius;
}Sphere;

Sphere* Sphere_Constructor(Vec3 center, float radius, Material material)
{
	Sphere* this = malloc(sizeof(Sphere));
	this->base = Obj_Constructor(ObjType_Sphere);
	this->center = center;
	this->radius = radius;
	this->base.material = material;
	return this;
}

static inline
BOOL Sphere_HitSphere(Sphere* this, Ray* ray, HitInfo* info)
{
	return Ray_HitSphere(ray, this->center, this->radius, info);
}

typedef struct Plane
{
	Obj base;
	Vec3 center, normal;
}Plane;

static const Num planeParallelThreshold = .0001f;

Plane* Plane_Constructor(Vec3 center, Vec3 normal, Material material)
{
	Plane* this = malloc(sizeof(Plane));
	this->base = Obj_Constructor(ObjType_Plane);
	this->center = center;
	this->normal = Vec3_Normalize(normal);
	this->base.material = material;
	return this;
}

BOOL Plane_HitPlane(Plane* this, Ray* ray, HitInfo* info)
{
	Num denom = Vec3_Dot2(this->normal, ray->direction);
	if (denom < -planeParallelThreshold)
	{
		Num t = Vec3_Dot2(Vec3_Sub(this->center, ray->origin), this->normal) / denom;
		if (t >= 0)
		{
			info->point = Vec3_Add(ray->origin, Vec3_Mul2(ray->direction, t));
			info->normal = this->normal;
			return TRUE;
		}
	}

	*info = HitInfo_Constructor();
	return FALSE;
}

BOOL Obj_Hit(Obj* this, Ray* ray, HitInfo* info)
{
	switch (this->type)
	{
		case ObjType_Sphere: return Sphere_HitSphere((Sphere*)this, ray, info);
		case ObjType_Plane: return Plane_HitPlane((Plane*)this, ray, info);
	}

	*info = HitInfo_Constructor();
	return FALSE;
}

typedef struct RawImage
{
	int width, height;
	Color* colors;
}RawImage;

RawImage* RawImage_Constructor(int width, int height)
{
	RawImage* this = malloc(sizeof(RawImage));
	this->width = width;
	this->height = height;
	this->colors = malloc(sizeof(Color) * width * height);
	return this;
}

typedef struct RawMovie
{
	int width, height;
	int frameCount;
	RawImage** frames;
}RawMovie;

RawMovie* RawMovie_Constructor(int width, int height, int frameCount)
{
	RawMovie* this = malloc(sizeof(RawMovie));
	this->width = width;
	this->height = height;
	this->frameCount = frameCount;
	this->frames = malloc(sizeof(RawImage*) * frameCount);
	for (int i = 0; i != frameCount; ++i)
	{
		this->frames[i] = RawImage_Constructor(width, height);
	}
	return this;
}

typedef struct Camera
{
	Vec3 position, forward, up;
	Mat3 matrix;
}Camera;

Camera* Camera_Constructor(Vec3 position, Vec3 forward, Vec3 up)
{
	Camera* this = malloc(sizeof(Camera));
	this->position = position;
	this->forward = Vec3_Normalize(forward);
	this->up = Vec3_Normalize(up);
	return this;
}

void Camera_Update(Camera* this)
{
	Vec3 f = Vec3_Neg(this->forward);
	Vec3 r = Vec3_Cross(this->up, f);
	Vec3 u = Vec3_Cross(f, r);
	this->matrix.x = Vec3_Normalize(r);
	this->matrix.y = Vec3_Normalize(u);
	this->matrix.z = Vec3_Normalize(f);
}

typedef struct PointLight
{
	Vec3 position;
	Num radius;
	Color color;

	Vec3 rotPos;
	Num rotSpeed;
}PointLight;

PointLight PointLight_Constructor(Vec3 position, Num radius, Color color, Num rotSpeed)
{
	PointLight this;
	this.position = position;
	this.radius = radius;
	this.color = color;

	this.rotPos = position;
	this.rotSpeed = rotSpeed;
	return this;
}

typedef struct Thread
{
#ifdef _WIN32
	thrd_t thread;
#else
	pthread_t thread;
#endif
}Thread;

typedef struct RenderThreadData
{
	int index, step;
	Thread thread;
	BOOL started;
}RenderThreadData;

BOOL Thread_Start(Thread* this, ThreadResult (*func)(void*), RenderThreadData* data)
{
#ifdef _WIN32
	return thrd_create(&this->thread, func, data) == thrd_success;
#else
	return pthread_create(&this->thread, NULL, func, data) == 0;
#endif
}

void Thread_Join(Thread* this)
{
#ifdef _WIN32
	thrd_join(this->thread, NULL);
#else
	pthread_join(this->thread, NULL);
#endif
}

enum { objectCount = 5, pointLightCount = 1 };

typedef struct Benchmark
{
	#define maxBounceCount 10// how many ray-bounces are allowed
	#define roughnessMul 1// force tween material value

	#define maxVolumeFogSteps (250 / 4)// how far can we test the shadow volume for
	#define volumeFogStep (.01f * 4)// how little the shadow volume testing step is
	#define fogDensity .4f// how dense the fog is
	#define fogFalloff 1
	Color fogColor;

	int width, height, frames;
	RawMovie* movie;
	Camera* camera;
	Obj* objects[objectCount];
	Color ambientLight;
	PointLight pointLights[pointLightCount];

	int threadCount;
}Benchmark;
Benchmark benchmark;

// forward declare
ThreadResult Benchmark_RenderThread(void* dataObj);
void Benchmark_UpdateMovieObjects(Camera* camera, PointLight* pointLights, int frame);
RawMovie* Benchmark_Render(BOOL singleThreaded);
BOOL Benchmark_IsInShadow(Vec3 lightPosition, Vec3 point, Num* lightDis, Vec3* lightDirection);
BOOL Benchmark_Trace(Ray* ray, Color* light, int* maxBounce, const Vec3* randomVec, const PointLight* pointLights);
void Benchmark_GatherLight(Obj* obj, const HitInfo* info, Color* light, const Vec3* randomVec, const PointLight* pointLights);

void Benchmark_Init(int width, int height, int frames)
{
	// init movie
	benchmark.width = width;
	benchmark.height = height;
	if (frames < 1) frames = 1;
	benchmark.frames = frames;
	benchmark.movie = RawMovie_Constructor(width, height, frames);

	// init camera
	benchmark.camera = Camera_Constructor(Vec3_Constructor2(0, 0, 0), Vec3_Constructor2(0, 0, -1), Vec3_Constructor2(0, 1, 0));
	Camera_Update(benchmark.camera);

	// init scene objects
	benchmark.objects[0] = (Obj*)Plane_Constructor(Vec3_Constructor2(0, -.4f, 0), Vec3_Constructor2(0, 1, 0), Material_Constructor2(Color_Constructor2(.3f, .3f, .3f), 0.03f * roughnessMul, 1, 0));
	benchmark.objects[1] = (Obj*)Sphere_Constructor(Vec3_Constructor2(0, 0, -2), .33f, Material_Constructor2(Color_Constructor2(1, 0, 0), 0 * roughnessMul, .25f, 0));
	benchmark.objects[2] = (Obj*)Sphere_Constructor(Vec3_Constructor2(-.25f, .5f, -1.5f), .25f, Material_Constructor2(Color_Constructor2(0, 1, 0), 0 * roughnessMul, 1, 1));
	benchmark.objects[3] = (Obj*)Sphere_Constructor(Vec3_Constructor2(1, -.75f, -2), 1, Material_Constructor2(Color_Constructor2(0, 0, 1), 0 * roughnessMul, 1, 1));
	benchmark.objects[4] = (Obj*)Sphere_Constructor(Vec3_Constructor2(.2f, .25f, -1.5f), .1f, Material_Constructor2(Color_Constructor2(1, 1, 0), 0 * roughnessMul, 1, .5f));

	// init scene lights
	benchmark.ambientLight = Color_Constructor2(.05f, .05f, .05f);
	benchmark.pointLights[0] = PointLight_Constructor(Vec3_Constructor2(.1f, .75f, -2), .01f, Color_Mul2(Color_Constructor2(1, 1, 1), 10), 1);

	// choose the number of render threads
	benchmark.threadCount = Environment_ProcessorCount();
	if (benchmark.threadCount < 1) benchmark.threadCount = 1;
	if (benchmark.threadCount > frames) benchmark.threadCount = frames;
	printf("Thread Count: %d\n", benchmark.threadCount);
}

void Benchmark_Dispose()
{
	for (int i = 0; i != benchmark.frames; ++i)
	{
		free(benchmark.movie->frames[i]->colors);
		free(benchmark.movie->frames[i]);
	}
	free(benchmark.movie->frames);
	free(benchmark.movie);

	for (int i = 0; i != objectCount; ++i) free(benchmark.objects[i]);
	free(benchmark.camera);
}

RawMovie* Benchmark_Render(BOOL singleThreaded)
{
	int threadCount = singleThreaded ? 1 : benchmark.threadCount;
	RenderThreadData* threadDatas = NULL;
	if (threadCount > 1) threadDatas = malloc(sizeof(RenderThreadData) * (threadCount - 1));
	if (threadDatas == NULL) threadCount = 1;

	RenderThreadData mainThread = {0};
	mainThread.step = threadCount;

	// each thread renders complete frames, interleaved across the movie
	for (int i = 1; i < threadCount; ++i)
	{
		RenderThreadData* data = &threadDatas[i - 1];
		data->index = i;
		data->step = threadCount;
		data->started = Thread_Start(&data->thread, Benchmark_RenderThread, data);
	}
	Benchmark_RenderThread(&mainThread);
	for (int i = 1; i < threadCount; ++i)
	{
		RenderThreadData* data = &threadDatas[i - 1];
		if (data->started) Thread_Join(&data->thread);
		else Benchmark_RenderThread(data);
	}
	free(threadDatas);

	return benchmark.movie;
}

void Benchmark_UpdateMovieObjects(Camera* camera, PointLight* pointLights, int frame)
{
	Num rot = frame * .1f;
	for (int l = 0; l != pointLightCount; ++l)
	{
		PointLight* pointLight = &pointLights[l];
		Num r = rot * pointLight->rotSpeed;
		pointLight->position = Vec3_Add(pointLight->rotPos, Vec3_Mul2(Vec3_Constructor2(MATH_Cos(r), MATH_Sin(r) * 4, MATH_Sin(r) * 3), .1f));
		pointLight->color = Color_Constructor2(MATH_Abs(MATH_Cos(r)) * 10, MATH_Abs(MATH_Sin(r)) * 10, MATH_Abs(MATH_Cos(r * .33f) * 10));
	}

	Num c = -rot * .25f;
	Vec3 center = Vec3_Constructor2(0, 0, -2);
	camera->position = Vec3_Add(center, Vec3_Mul2(Vec3_Constructor2(MATH_Sin(c), MATH_Sin(c) * .1f, -MATH_Cos(c)), 2));
	camera->forward = Vec3_Normalize(Vec3_Sub(center, camera->position));
	Camera_Update(camera);
}

ThreadResult Benchmark_RenderThread(void* dataObj)
{
	RenderThreadData* data = (RenderThreadData*)dataObj;
	Camera camera = *benchmark.camera;
	PointLight pointLights[pointLightCount];
	for (int l = 0; l != pointLightCount; ++l) pointLights[l] = benchmark.pointLights[l];
	Num widthRange = (Num)(benchmark.width - 1);
	Num heightRange = (Num)(benchmark.height - 1);

	for (int f = data->index; f < benchmark.frames; f += data->step)
	{
		printf("Frame: %d rendering\n", f);
		Benchmark_UpdateMovieObjects(&camera, pointLights, f);

		// each frame has its own random sequence regardless of which thread renders it
		RandomUtil random = {f * .5f, f * .75f};
		Ray ray;
		ray.origin = camera.position;
		Color* colors = benchmark.movie->frames[f]->colors;
		for (int y = 0; y < benchmark.height; ++y)
		{
			Num v = y / heightRange;
			for (int x = 0; x < benchmark.width; ++x)
			{
				Num u = x / widthRange;
				ray.direction = Vec3_Normalize(Vec3_Constructor2(u - .5f, v - .5f, -1));
				ray.direction = Mat3_Transform(camera.matrix, ray.direction);

				// generate random vector
				Num randomVal = RandomUtil_NextValue(&random, u, v) - .5f;
				Vec3 randomVec = Vec3_Constructor2(randomVal, randomVal, randomVal);

				// trace
				int maxBounce = maxBounceCount;
				Color light = Color_Constructor();
				Benchmark_Trace(&ray, &light, &maxBounce, &randomVec, pointLights);

				// write result
				*colors++ = light;
			}
		}
	}
	return 0;
}

static inline
Num Benchmark_LightFalloff(Num lightDis)
{
	Num value = lightDis + 1;
	Num squared = value * value;
	return squared * squared;
}

#if !DISABLE_FOG
static inline
void Benchmark_GatherFog(const Ray* ray, Color* light, const Vec3* randomVec, int steps, const PointLight* pointLights)
{
	Vec3 fogStep = Vec3_Mul2(ray->direction, volumeFogStep);
	for (int l = 0; l != pointLightCount; ++l)
	{
		Vec3 fogPos = ray->origin;
		Num fogBrightness = 0;
		const PointLight* pointLight = &pointLights[l];
		Vec3 lightPosition = Vec3_Add(pointLight->position, Vec3_Mul2(*randomVec, pointLight->radius));
		for (int i = 0; i != steps; ++i)
		{
			fogPos = Vec3_Add(fogPos, fogStep);
			Num lightDis;
			if (!Benchmark_IsInShadow(lightPosition, fogPos, &lightDis, NULL)) fogBrightness += 1 / Benchmark_LightFalloff(lightDis);
		}
		*light = Color_Add(*light, Color_Mul2(pointLight->color, fogBrightness * volumeFogStep * .1f));
	}
}
#endif

BOOL Benchmark_Trace(Ray* ray, Color* light, int* maxBounce, const Vec3* randomVec, const PointLight* pointLights)
{
	// find closes object
	Obj* closestObject = NULL;
	Num closestDis = MATH_MaxValue();
	HitInfo closestInfo = HitInfo_Constructor();
	for (int o = 0; o != objectCount; ++o)
	{
		Obj* obj = benchmark.objects[o];
		HitInfo info;
		if (Obj_Hit(obj, ray, &info))
		{
			Num dis = Vec3_Length(Vec3_Sub(ray->origin, info.point));
			if (dis < closestDis)
			{
				closestDis = dis;
				closestObject = obj;
				closestInfo = info;
			}
		}
	}

	// hit point light orb
	for (int l = 0; l != pointLightCount; ++l)
	{
		const PointLight* pointLight = &pointLights[l];
		const PointLight* closestLight = NULL;
		HitInfo info;
		if (Ray_HitSphere(ray, pointLight->position, pointLight->radius, &info))
		{
			Num dis = Vec3_Length(Vec3_Sub(ray->origin, info.point));
			if (dis < closestDis)
			{
				closestDis = dis;
				closestLight = pointLight;
				closestInfo = info;
			}
		}

		if (closestLight != NULL)
		{
			*light = closestLight->color;
			return TRUE;
		}
	}

	// process lighting
	if (closestObject != NULL)
	{
		if (*maxBounce <= 0) return FALSE;
		(*maxBounce)--;
		int maxBounceReset = *maxBounce;

		// buffer diffuse pass state objects
		HitInfo reflectedClosestInfo = closestInfo;
		Ray reflectedRay = *ray;

		// diffuse surface normal
		reflectedClosestInfo.normal = Vec3_Add(closestInfo.normal, Vec3_Div2(Vec3_Mul2(*randomVec, closestObject->material.roughness), closestDis + 1));
		reflectedRay.direction = Vec3_Normalize(Vec3_Reflect(ray->direction, reflectedClosestInfo.normal));
		reflectedRay.origin = Vec3_Sub(closestInfo.point, Vec3_Mul2(reflectedRay.direction, .0001f));// move a little off surface to correct for floating point errors

		// gather surface light
		Benchmark_GatherLight(closestObject, &reflectedClosestInfo, light, randomVec, pointLights);

		// trace from this reflected path
		*maxBounce = maxBounceReset;// reset max bounce here or we lose light average consistancy
		Color reflectedLight = Color_Constructor();
		Benchmark_Trace(&reflectedRay, &reflectedLight, maxBounce, randomVec, pointLights);

		// material (reflection)
		Num fresnel = 1 - Vec3_FresnelSchlickZero(ray->direction, closestInfo.normal);
		*light = Color_Add(*light, Color_Mul2(Color_Lerp(reflectedLight, Color_Mul(reflectedLight, closestObject->material.color), closestObject->material.metalic * fresnel), closestObject->material.reflectiveness));

		// volume fog tracing taking into account shadows
		#if !DISABLE_FOG
		int maxVolumeFogStepsClamped = (int)MATH_Min(closestDis / volumeFogStep, maxVolumeFogSteps);
		Benchmark_GatherFog(ray, light, randomVec, maxVolumeFogStepsClamped, pointLights);
		#endif
				
		return TRUE;
	}
	else
	{
		// volume fog tracing taking into account shadows (optimized for when nothing is hit)
		#if !DISABLE_FOG
		Benchmark_GatherFog(ray, light, randomVec, maxVolumeFogSteps, pointLights);
		#endif
	}

	// hit nothing
	return FALSE;
}

void Benchmark_GatherLight(Obj* obj, const HitInfo* info, Color* light, const Vec3* randomVec, const PointLight* pointLights)
{
	Color objColor = obj->material.color;

	// ambient
	*light = Color_Add(*light, Color_Mul(objColor, benchmark.ambientLight));

	// point
	for (int l = 0; l != pointLightCount; ++l)
	{
		const PointLight* pointLight = &pointLights[l];

		// diffuse
		Vec3 lightPosition = Vec3_Add(pointLight->position, Vec3_Mul2(*randomVec, pointLight->radius));
		Num lightDis;
		Vec3 lightDirection;
		if (!Benchmark_IsInShadow(lightPosition, info->point, &lightDis, &lightDirection))
		{
			Num d = MATH_Max(Vec3_Dot2(info->normal, lightDirection), 0);
			d /= Benchmark_LightFalloff(lightDis);
			*light = Color_Add(*light, Color_Mul2(Color_Mul(objColor, pointLight->color), d));
		}
	}
}

BOOL Benchmark_IsInShadow(Vec3 lightPosition, Vec3 point, Num* lightDis, Vec3* lightDirection)
{
	// use a ray whose parameter is 1 at the light, so shadow tests need no normalization
	Ray lightRay = Ray_Constructor2(point, Vec3_Sub(lightPosition, point));
	Num a = Vec3_Dot(lightRay.direction);
	for (int o = 0; o != objectCount; ++o)
	{
		Obj* obj = benchmark.objects[o];
		switch (obj->type)
		{
			case ObjType_Sphere:
			{
				Sphere* sphere = (Sphere*)obj;
				Num numerator;
				// The hit parameter is numerator / a.
				if (Ray_HitSphereNumerator(&lightRay, sphere->center, sphere->radius, a, &numerator) && MATH_Abs(numerator) <= a) return TRUE;
				break;
			}
			case ObjType_Plane:
			{
				Plane* plane = (Plane*)obj;
				Num denom = Vec3_Dot2(plane->normal, lightRay.direction);
				if (denom < 0 && denom * denom > a * (planeParallelThreshold * planeParallelThreshold))
				{
					Num offset = Vec3_Dot2(Vec3_Sub(plane->center, point), plane->normal);
					if (offset <= 0 && offset >= denom) return TRUE;
				}
				break;
			}
		}
	}

	*lightDis = MATH_Sqrt(a);
	if (lightDirection != NULL) *lightDirection = Vec3_Div2(lightRay.direction, *lightDis);
	return FALSE;
}