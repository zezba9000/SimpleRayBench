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

typedef struct RandomUtil
{
	Num offsetX, offsetY;
}RandomUtil;
RandomUtil _RandomUtil;

inline
Num RandomUtil_NextValue(Num u, Num v)
{
	#define randoValueX 12.9898f
	#define randoValueY 78.233f
	#define randoValueZ 43758.543123f

	u += _RandomUtil.offsetX;
	v += _RandomUtil.offsetY;
	_RandomUtil.offsetX += .5f;
	_RandomUtil.offsetY += .75f;

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

inline
Color Color_Add(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b);
}

inline
Color Color_Sub(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b);
}

inline
Color Color_Mul(Color p1, const Color p2)
{
	return Color_Constructor2(p1.r * p2.r, p1.g * p2.g, p1.b * p2.b);
}

inline
Color Color_Mul2(Color this, Num p2)
{
	return Color_Constructor2(this.r * p2, this.g * p2, this.b * p2);
}

inline
Color Color_Div(Color this, const Color p1, Num p2)
{
	return Color_Constructor2(p1.r / p2, p1.g / p2, p1.b / p2);
}

inline
Color Color_Lerp(Color p1, Color p2, Num t)
{
	return Color_Add(p1, Color_Mul2(Color_Sub(p2, p1), t));
}

inline
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

inline
Vec3 Vec3_Add(Vec3 this, const Vec3 p2)
{
	return Vec3_Constructor2(this.x + p2.x, this.y + p2.y, this.z + p2.z);
}

inline
Vec3 Vec3_Sub(Vec3 this, const Vec3 p2)
{
	return Vec3_Constructor2(this.x - p2.x, this.y - p2.y, this.z - p2.z);
}

inline
Vec3 Vec3_Sub2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x - p2, p1.y - p2, p1.z - p2);
}

inline
Vec3 Vec3_Neg(Vec3 v)
{
	return Vec3_Constructor2(-v.x, -v.y, -v.z);
}

inline
Vec3 Vec3_Mul(Vec3 p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}

inline
Vec3 Vec3_Mul2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x * p2, p1.y * p2, p1.z * p2);
}

inline
Vec3 Vec3_Mul3(Num p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1 * p2.x, p1 * p2.y, p1 * p2.z);
}

inline
Vec3 Vec3_Div(Vec3 p1, const Vec3 p2)
{
	return Vec3_Constructor2(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
}

inline
Vec3 Vec3_Div2(const Vec3 p1, Num p2)
{
	return Vec3_Constructor2(p1.x / p2, p1.y / p2, p1.z / p2);
}

inline
Num Vec3_Dot(Vec3 this)
{
	return (this.x * this.x) + (this.y * this.y) + (this.z * this.z);
}

inline
Num Vec3_Dot2(Vec3 this, Vec3 vec)
{
	return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
}

inline
Num Vec3_Length(Vec3 this)
{
	return MATH_Sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
}

inline
Vec3 Vec3_Normalize(Vec3 this)
{
	return Vec3_Div2(this, MATH_Sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z)));
}

inline
Vec3 Vec3_Reflect(Vec3 this, Vec3 planeNormal)
{
	return Vec3_Sub(this, Vec3_Mul2(planeNormal, Vec3_Dot2(this, planeNormal) * 2));
}

inline
Vec3 Vec3_Cross(Vec3 this, Vec3 vector)
{
	return Vec3_Constructor2(((this.y * vector.z) - (this.z * vector.y)), ((this.z * vector.x) - (this.x * vector.z)), ((this.x * vector.y) - (this.y * vector.x)));
}

inline
Num Vec3_FresnelSchlickZero(Vec3 rayDirection, Vec3 normal)
{
	return MATH_Pow(1 - MATH_Max(Vec3_Dot2(Vec3_Neg(rayDirection), normal), 0), 5);
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

BOOL Ray_HitSphere(Ray* this, Vec3 sphereCenter, Num sphereRadius, HitInfo* info)
{
	// if ray pointing wrong direction ignore
	if (Vec3_Dot2(this->direction, Vec3_Sub(this->origin, sphereCenter)) >= 0)
	{
		*info = HitInfo_Constructor();
		return FALSE;
	}

	// compute
	Vec3 d = Vec3_Sub(this->origin, sphereCenter);
	Num a = Vec3_Dot2(this->direction, this->direction);
	Num b = Vec3_Dot2(d, this->direction);
	Num c = Vec3_Dot(d) - sphereRadius * sphereRadius;

	// if ray not pointing at object ignore
	Num disc = b * b - a * c;
	if (disc < 0)
	{
		*info = HitInfo_Constructor();
		return FALSE;
	}

	// compute
	Num sqrtDisc = MATH_Sqrt(disc);
	Num invA = 1 / a;
	Num t1 = (-b - sqrtDisc) * invA;

	info->point = Vec3_Add(this->origin, Vec3_Mul3(t1, this->direction));
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

Obj* Obj_Constructor()
{
	Obj* this = malloc(sizeof(Obj));
	this->material = Material_Constructor();
	this->type = ObjType_Plane;
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
	this->center = center;
	this->radius = radius;
	this->base.material = material;
	return this;
}

inline
BOOL Sphere_HitSphere(Sphere* this, Ray* ray, HitInfo* info)
{
	return Ray_HitSphere(ray, this->center, this->radius, info);
}

typedef struct Plane
{
	Obj base;
	Vec3 center, normal;
}Plane;

Plane* Plane_Constructor(Vec3 center, Vec3 normal, Material material)
{
	Plane* this = malloc(sizeof(Plane));
	this->center = center;
	this->normal = Vec3_Normalize(normal);
	this->base.material = material;
	return this;
}

BOOL Plane_HitPlane(Plane* this, Ray* ray, HitInfo* info)
{
	// if ray pointing wrong direction ignore
	if (Vec3_Dot2(ray->direction, this->normal) >= 0)
	{
		*info = HitInfo_Constructor();
		return FALSE;
	}

	Num denom = Vec3_Dot2(this->normal, ray->direction);
	if (MATH_Abs(denom) > 0.0001f)
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
		case ObjType_Plane: return Sphere_HitSphere((Sphere*)this, ray, info);
		case ObjType_Sphere: return Plane_HitPlane((Plane*)this, ray, info);
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
	this->frames = malloc(sizeof(RawImage) * frameCount);
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

PointLight* PointLight_Constructor(Vec3 position, Num radius, Color color, Num rotSpeed)
{
	PointLight* this = malloc(sizeof(PointLight));
	this->position = position;
	this->radius = radius;
	this->color = color;

	this->rotPos = position;
	this->rotSpeed = rotSpeed;
	return this;
}

typedef struct RenderThreadData
{
	int index, randomIndex;
	BOOL waiting, done, exit;
	int x, y, xEnd, yEnd;
	RawImage* image;
}RenderThreadData;

typedef struct Thread
{
	thrd_t thread;
}Thread;

Thread Thread_Constructor(thrd_start_t func, RenderThreadData* data)
{
	Thread this;
	thrd_create(&this.thread, func, data);
	return this;
}

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
	int objectsLength;
	Obj** objects;
	Color ambientLight;
	int pointLightsLength;
	PointLight** pointLights;

	int threadCount;
	RenderThreadData** threadDatas;
	Thread* threads;
	BOOL threadsAlive;
}Benchmark;
Benchmark benchmark;

// forward declare
void Benchmark_RenderThread(void* dataObj);
void Benchmark_UpdateMovieObjects(int frame);
RawMovie* Benchmark_Render(BOOL singleThreaded);
BOOL Benchmark_IsInShadow(PointLight* pointLight, const Vec3* point, const Vec3* randomVec, Num* lightDis, Ray* lightRay);
BOOL Benchmark_Trace(Ray* ray, Color* light, int* maxBounce, const Vec3* randomVec);
void Benchmark_GatherLight(Obj* obj, const HitInfo* info, Color* light, const Vec3* randomVec);

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

	// init scene objects
	benchmark.objectsLength = 5;
	benchmark.objects = malloc(sizeof(Obj*) * benchmark.objectsLength);
	benchmark.objects[0] = Plane_Constructor(Vec3_Constructor2(0, -.4f, 0), Vec3_Constructor2(0, 1, 0), Material_Constructor2(Color_Constructor2(.3f, .3f, .3f), 0.03f * roughnessMul, 1, 0));
	benchmark.objects[1] = Sphere_Constructor(Vec3_Constructor2(0, 0, -2), .33f, Material_Constructor2(Color_Constructor2(1, 0, 0), 0 * roughnessMul, .25f, 0));
	benchmark.objects[2] = Sphere_Constructor(Vec3_Constructor2(-.25f, .5f, -1.5f), .25f, Material_Constructor2(Color_Constructor2(0, 1, 0), 0 * roughnessMul, 1, 1));
	benchmark.objects[3] = Sphere_Constructor(Vec3_Constructor2(1, -.75f, -2), 1, Material_Constructor2(Color_Constructor2(0, 0, 1), 0 * roughnessMul, 1, 1));
	benchmark.objects[4] = Sphere_Constructor(Vec3_Constructor2(.2f, .25f, -1.5f), .1f, Material_Constructor2(Color_Constructor2(1, 1, 0), 0 * roughnessMul, 1, .5f));

	// init scene lights
	benchmark.ambientLight = Color_Constructor2(.05f, .05f, .05f);
	benchmark.pointLightsLength = 1;
	benchmark.pointLights = malloc(sizeof(PointLight*) * benchmark.pointLightsLength);
	benchmark.pointLights[0] = PointLight_Constructor(Vec3_Constructor2(.1f, .75f, -2), .01f, Color_Mul2(Color_Constructor2(1, 1, 1), 10), 1);

	// start threads
	benchmark.threadsAlive = TRUE;
	benchmark.threadCount = Environment_ProcessorCount();
	printf("Thread Count: %d\n", benchmark.threadCount);
	benchmark.threadDatas = malloc(sizeof(RenderThreadData*) * benchmark.threadCount);
	benchmark.threads = malloc(sizeof(Thread) * benchmark.threadCount);
	for (int i = 0; i != benchmark.threadCount; ++i)
	{
		int sectionHeight = height / benchmark.threadCount;
		benchmark.threadDatas[i] = malloc(sizeof(RenderThreadData));
		benchmark.threadDatas[i]->exit = FALSE;
		benchmark.threadDatas[i]->waiting = TRUE;
		benchmark.threadDatas[i]->done = FALSE;
		benchmark.threadDatas[i]->index = i,
		benchmark.threadDatas[i]->randomIndex = i % benchmark.threadCount,
		benchmark.threadDatas[i]->x = 0,
		benchmark.threadDatas[i]->y = i * sectionHeight,
		benchmark.threadDatas[i]->xEnd = width,
		benchmark.threadDatas[i]->yEnd = (i * sectionHeight) + sectionHeight;
		if (i == benchmark.threadCount - 1) benchmark.threadDatas[i]->yEnd = height;
		benchmark.threads[i] = Thread_Constructor(Benchmark_RenderThread, benchmark.threadDatas[i]);
	}
}

void Benchmark_Dispose()
{
	for (int i = 0; i != benchmark.threadCount; ++i)
	{
		RenderThreadData* data = benchmark.threadDatas[i];
		data->exit = TRUE;
		data->done = TRUE;
		data->waiting = FALSE;
	}
}

RawMovie* Benchmark_Render(BOOL singleThreaded)
{
	if (singleThreaded)
	{
		// setup ray
		Ray ray;
		ray.origin = benchmark.camera->position;

		// render frames
		Num widthRange = benchmark.width - 1;
		Num heightRange = benchmark.height - 1;
		for (int f = 0; f != benchmark.frames; ++f)
		{
			printf("Frame: %d rendering\n", f);
			Benchmark_UpdateMovieObjects(f);
			Color* colors = benchmark.movie->frames[f]->colors;
			for (int y = 0; y != benchmark.height; ++y)
			{
				for (int x = 0; x != benchmark.width; ++x)
				{
					ray.direction = Vec3_Normalize(Vec3_Constructor2((x / (Num)(benchmark.width - 1)) - .5f, (y / (Num)(benchmark.height - 1)) - .5f, -1));
					ray.direction = Mat3_Transform(benchmark.camera->matrix, ray.direction);

					// generate shared random vector
					Num randomVal = RandomUtil_NextValue(x / widthRange, y / heightRange);
					Vec3 randomVec = Vec3_Sub2(Vec3_Constructor2(randomVal, randomVal, randomVal), .5f);

					// trace
					int maxBounce = maxBounceCount;
					Color light = Color_Constructor();
					Benchmark_Trace(&ray, &light, maxBounce, &randomVec);

					// write result
					colors[x + (y * benchmark.width)] = light;
				}
			}
		}
	}
	else
	{
		// render frames
		for (int f = 0; f != benchmark.frames; ++f)
		{
			printf("Frame: %d rendering\n", f);
			Benchmark_UpdateMovieObjects(f);
			RawImage* frame = benchmark.movie->frames[f];

			// tell threads to start processing task
			for (int i = 0; i != benchmark.threadCount; ++i)
			{
				RenderThreadData* data = benchmark.threadDatas[i];
				data->image = frame;
				data->done = FALSE;
				data->waiting = FALSE;
			}

			// wait for all threads to finish
			for (int i = 0; i != benchmark.threadCount; ++i)
			{
				RenderThreadData* data = benchmark.threadDatas[i];
				while (!data->done) Sleep(1);
			}
		}
	}

	return benchmark.movie;
}

void Benchmark_UpdateMovieObjects(int frame)
{
	Num rot = frame * .1f;
	for (int l = 0; l != benchmark.pointLightsLength; ++l)
	{
		PointLight* pointLight = benchmark.pointLights[l];
		Num r = rot * pointLight->rotSpeed;
		pointLight->position = Vec3_Add(pointLight->rotPos, Vec3_Mul2(Vec3_Constructor2(MATH_Cos(r), MATH_Sin(r) * 4, MATH_Sin(r) * 3), .1f));
		pointLight->color = Color_Constructor2(MATH_Abs(MATH_Cos(r)) * 10, MATH_Abs(MATH_Sin(r)) * 10, MATH_Abs(MATH_Cos(r * .33f) * 10));
	}

	Num c = -rot * .25f;
	Vec3 center = Vec3_Constructor2(0, 0, -2);
	benchmark.camera->position = Vec3_Add(center, Vec3_Mul2(Vec3_Constructor2(MATH_Sin(c), MATH_Sin(c) * .1f, -MATH_Cos(c)), 2));
	benchmark.camera->forward = Vec3_Normalize(Vec3_Sub(center, benchmark.camera->position));
	Camera_Update(benchmark.camera);
}

void Benchmark_RenderThread(void* dataObj)
{
	RenderThreadData* data = (RenderThreadData*)dataObj;

	while (!data->exit)
	{
		// wait for main thread to trigger
		while (data->waiting) Sleep(1);
		if (data->done) return;

		// setup ray
		Ray ray;
		ray.origin = benchmark.camera->position;

		// render image section
		Color* colors = data->image->colors;
		Num widthRange = benchmark.width - 1;
		Num heightRange = benchmark.height - 1;
		for (int y = data->y; y < data->yEnd; ++y)
		{
			for (int x = data->x; x < data->xEnd; ++x)
			{
				ray.direction = Vec3_Normalize(Vec3_Constructor2((x / (Num)(benchmark.width - 1)) - .5f, (y / (Num)(benchmark.height - 1)) - .5f, -1));
				ray.direction = Mat3_Transform(benchmark.camera->matrix, ray.direction);

				// generate shared random vector
				Num randomVal = RandomUtil_NextValue(x / widthRange, y / heightRange);
				Vec3 randomVec = Vec3_Sub2(Vec3_Constructor2(randomVal, randomVal, randomVal), .5f);

				// trace
				int maxBounce = maxBounceCount;
				Color light = Color_Constructor();
				Benchmark_Trace(&ray, &light, maxBounce, &randomVec);

				// write result
				colors[x + (y * benchmark.width)] = light;
			}
		}

		// tell main thread we're done
		data->done = TRUE;
		data->waiting = TRUE;
	}
}

BOOL Benchmark_Trace(Ray* ray, Color* light, int* maxBounce, const Vec3* randomVec)
{
	// find closes object
	Obj* closestObject = NULL;
	Num closestDis = MATH_MaxValue();
	HitInfo closestInfo = HitInfo_Constructor();
	for (int o = 0; o != benchmark.objectsLength; ++o)
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
	for (int l = 0; l != benchmark.pointLightsLength; ++l)
	{
		PointLight* pointLight = benchmark.pointLights[l];
		PointLight* closestLight = NULL;
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
		if (maxBounce <= 0) return FALSE;
		maxBounce--;
		int maxBounceReset = maxBounce;

		// buffer diffuse pass state objects
		HitInfo reflectedClosestInfo = closestInfo;
		Ray reflectedRay = *ray;

		// diffuse surface normal
		reflectedClosestInfo.normal = Vec3_Add(closestInfo.normal, Vec3_Div2(Vec3_Mul2(*randomVec, closestObject->material.roughness), closestDis + 1));
		reflectedRay.direction = Vec3_Normalize(Vec3_Reflect(ray->direction, reflectedClosestInfo.normal));
		reflectedRay.origin = Vec3_Sub(closestInfo.point, Vec3_Mul2(reflectedRay.direction, .0001f));// move a little off surface to correct for floating point errors

		// gather surface light
		Benchmark_GatherLight(closestObject, &reflectedClosestInfo, light, randomVec);

		// trace from this reflected path
		maxBounce = maxBounceReset;// reset max bounce here or we lose light average consistancy
		Color reflectedLight = Color_Constructor();
		Benchmark_Trace(&reflectedRay, &reflectedLight, maxBounce, randomVec);

		// material (reflection)
		Num fresnel = 1 - Vec3_FresnelSchlickZero(ray->direction, closestInfo.normal);
		*light = Color_Add(*light, Color_Mul2(Color_Lerp(reflectedLight, Color_Mul(reflectedLight, closestObject->material.color), closestObject->material.metalic * fresnel), closestObject->material.reflectiveness));

		// volume fog tracing taking into account shadows
		#if !DISABLE_FOG
		Vec3 fogPos = ray->origin;
		Vec3 fogStep = Vec3_Mul2(ray->direction, volumeFogStep);
		int maxVolumeFogStepsClamped = (int)MATH_Min(closestDis / volumeFogStep, maxVolumeFogSteps);
		for (int l = 0; l != benchmark.pointLightsLength; ++l)
		{
			Num fogBrightness = 0;
			PointLight* pointLight = benchmark.pointLights[l];
			for (int i = 0; i != maxVolumeFogStepsClamped; ++i)
			{
				fogPos = Vec3_Add(fogPos, fogStep);
				Num lightDis;
				Ray lightRay;
				if (!Benchmark_IsInShadow(pointLight, &fogPos, randomVec, &lightDis, &lightRay)) fogBrightness += 1 / MATH_Pow(lightDis + 1, 4);
			}
			*light = Color_Add(*light, Color_Mul2(pointLight->color, fogBrightness * volumeFogStep * .1f));
		}
		#endif
				
		return TRUE;
	}
	else
	{
		// volume fog tracing taking into account shadows (optimized for when nothing is hit)
		#if !DISABLE_FOG
		Vec3 fogPos = ray->origin;
		Vec3 fogStep = Vec3_Mul2(ray->direction, volumeFogStep);
		for (int l = 0; l != benchmark.pointLightsLength; ++l)
		{
			Num fogBrightness = 0;
			PointLight* pointLight = benchmark.pointLights[l];
			for (int i = 0; i != maxVolumeFogSteps; ++i)
			{
				fogPos = Vec3_Add(fogPos, fogStep);
				Num lightDis;
				Ray lightRay;
				if (!Benchmark_IsInShadow(pointLight, &fogPos, randomVec, &lightDis, &lightRay)) fogBrightness += 1 / MATH_Pow(lightDis + 1, 4);
			}
			*light = Color_Add(*light, Color_Mul2(pointLight->color, fogBrightness * volumeFogStep * .1f));
		}
		#endif
	}

	// hit nothing
	return FALSE;
}

void Benchmark_GatherLight(Obj* obj, const HitInfo* info, Color* light, const Vec3* randomVec)
{
	Color objColor = obj->material.color;

	// ambient
	*light = Color_Add(*light, Color_Mul(objColor, benchmark.ambientLight));

	// point
	for (int l = 0; l != benchmark.pointLightsLength; ++l)
	{
		PointLight* pointLight = benchmark.pointLights[l];

		// diffuse
		Num lightDis;
		Ray lightRay;
		if (!Benchmark_IsInShadow(benchmark.pointLights[l], &info->point, randomVec, &lightDis, &lightRay))
		{
			Num d = MATH_Max(Vec3_Dot2(info->normal, lightRay.direction), 0);
			d /= MATH_Pow(lightDis + 1, 4);
			*light = Color_Add(*light, Color_Mul2(Color_Mul(objColor, pointLight->color), d));
		}
	}
}

BOOL Benchmark_IsInShadow(PointLight* pointLight, const Vec3* point, const Vec3* randomVec, Num* lightDis, Ray* lightRay)
{
	Vec3 softVec = Vec3_Sub(Vec3_Add(pointLight->position, Vec3_Mul2(*randomVec, pointLight->radius)), *point);
	*lightDis = Vec3_Length(softVec);
	*lightRay = Ray_Constructor2(*point, Vec3_Normalize(softVec));
	for (int o = 0; o != benchmark.objectsLength; ++o)
	{
		Obj* obj = benchmark.objects[o];
		HitInfo hitInfo;
		if (Obj_Hit(obj, lightRay, &hitInfo) && Vec3_Length(Vec3_Sub(hitInfo.point, *point)) <= *lightDis)
		{
			return TRUE;
		}
	}
	return FALSE;
}