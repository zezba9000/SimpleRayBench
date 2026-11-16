#pragma once
#include "LangUtils.h"

#define NUM32
//#define NUM64
//#define DISABLE_FOG

#if defined(NUM32)
#define Num float
#define MATH MathF
#elif defined(NUM64)
#define Num double
#define MATH Math
#endif

typedef struct RandomUtil
{
	float offsetX, offsetY;
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
	return Color_Add(p1, Color_Mul(Color_Sub(p2, p1), MATH_Clamp(t, 0, 1)));
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
	return Vec3_Div(this, MATH_Sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z)));
}

inline
Vec3 Vec3_Reflect(Vec3 this, Vec3 planeNormal)
{
	return Vec3_Sub(this, Vec3_Mul2(planeNormal, Vec3_Mul2(Vec3_Dot(this, planeNormal), 2));
}

inline
Vec3 Vec3_Cross(Vec3 this, Vec3 vector)
{
	return Vec3_Constructor2(((this.y * vector.z) - (this.z * vector.y)), ((this.z * vector.x) - (this.x * vector.z)), ((this.x * vector.y) - (this.y * vector.x)));
}

inline
Num Vec3_FresnelSchlickZero(Vec3 this, Vec3 rayDirection, Vec3 normal)
{
	return MATH_Pow(1 - MATH_Max(Vec3_Dot(Vec3_Neg(rayDirection), normal), 0), 5);
}

typedef struct Mat3
{
	Vec3 x, y, z;
}Mat3;

Vec3 Mat3_Transform(Vec3 this, Vec3 v)
{
	return (this.x * v.x) + (this.y * v.y) + (this.z * v.z);
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
	//Num t2 = (-b + sqrtDisc) * invA;

	info->point = Vec3_Add(this->origin, Vec3_Mul3(t1, this->direction));
	//info.point = origin + t2 * direction;
	info->normal = Vec3_Normalize(Vec3_Sub(info->point, sphereCenter));

	return TRUE;
}

typedef struct Obj
{
	Color color;
	Num roughness;
	Num reflectiveness, metalic;
}Obj;

Obj* Obj_Constructor(Color color, Num roughness, Num reflectiveness, Num metalic)
{
	Obj* this = malloc(sizeof(Obj));
	this->color = color;
	this->roughness = roughness;
	this->reflectiveness = reflectiveness;
	this->metalic = metalic;
	return this;
}

virtual BOOL Obj_Hit(Ray* ray, HitInfo* info)
{
	return false;
}

class Sphere : public Obj
{
public:
	Vec3 center;
	float radius;

	Sphere(Vec3 center, float radius, Color color, Num roughness, Num reflectiveness, Num metalic)
	: Obj(color, roughness, reflectiveness, metalic)
	{
		this->center = center;
		this->radius = radius;
	}

	virtual bool Hit(Ray& ray, HitInfo& info)
	{
		return ray.HitSphere(center, radius, info);
	}
};

class Plane : public Obj
{
public:
	Vec3 center, normal;

	Plane(Vec3 center, Vec3 normal, Color color, Num roughness, Num reflectiveness, Num metalic)
	: Obj(color, roughness, reflectiveness, metalic)
	{
		this->center = center;
		this->normal = normal.Normalize();
	}

	virtual bool Hit(Ray& ray, HitInfo& info)
	{
		// if ray pointing wrong direction ignore
		if (ray.direction.Dot(normal) >= 0)
		{
			info = HitInfo();
			return false;
		}

		Num denom = normal.Dot(ray.direction);
		if (MATH::Abs(denom) > 0.0001f)
		{
			Num t = (center - ray.origin).Dot(normal) / denom;
			if (t >= 0)
			{
				info.point = ray.origin + (ray.direction * t);
				info.normal = normal;
				return true;
			}
		}

		info = HitInfo();
		return false;
	}
};

class RawImage
{
public:
	int width, height;
	Color* colors;

	RawImage(int width, int height)
	{
		this->width = width;
		this->height = height;
		colors = new Color[width * height];
	}
};

class RawMovie
{
public:
	int width, height;
	int frameCount;
	RawImage** frames;

	RawMovie(int width, int height, int frameCount)
	{
		this->width = width;
		this->height = height;
		this->frameCount = frameCount;
		frames = new RawImage*[frameCount];
		for (int i = 0; i != frameCount; ++i)
		{
			frames[i] = new RawImage(width, height);
		}
	}
};

class Camera
{
public:
	Vec3 position, forward, up;
	Mat3 matrix;

	Camera(Vec3 position, Vec3 forward, Vec3 up)
	{
		this->position = position;
		this->forward = forward.Normalize();
		this->up = up.Normalize();
	}

	void Update()
	{
		auto f = -forward;
		auto r = up.Cross(f);
		auto u = f.Cross(r);
		matrix.x = r.Normalize();
		matrix.y = u.Normalize();
		matrix.z = f.Normalize();
	}
};

class PointLight
{
public:
	Vec3 position;
	Num radius;
	Color color;

	Vec3 rotPos;
	Num rotSpeed;

	PointLight(Vec3 position, Num radius, Color color, Num rotSpeed)
	{
		this->position = position;
		this->radius = radius;
		this->color = color;

		rotPos = position;
		this->rotSpeed = rotSpeed;
	}
};

class RenderThreadData
{
public:
	int index, randomIndex;
	bool waiting, done, exit;
	int x, y, xEnd, yEnd;
	RawImage* image;
};

class Benchmark
{
public:
	#define maxBounceCount 10// how many ray-bounces are allowed
	#define roughnessMul 1// force tween material value

	#define maxVolumeFogSteps (250 / 4)// how far can we test the shadow volume for
	#define volumeFogStep (.01f * 4)// how little the shadow volume testing step is
	#define fogDensity .4f// how dense the fog is
	#define fogFalloff 1
	static Color fogColor;

	static int width, height, frames;
	static RawMovie* movie;
	static Camera* camera;
	static int objectsLength;
	static Obj** objects;
	static Color ambientLight;
	static int pointLightsLength;
	static PointLight** pointLights;

	static int threadCount;
	static RenderThreadData** threadDatas;
	static std::thread** threads;
	static bool threadsAlive;

	static void Init(int width, int height, int frames)
	{
		// init movie
		Benchmark::width = width;
		Benchmark::height = height;
		if (frames < 1) frames = 1;
		Benchmark::frames = frames;
		movie = new RawMovie(width, height, frames);

		// init camera
		camera = new Camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0));

		// init scene objects
		objectsLength = 5;
		objects = new Obj*[objectsLength];
		objects[0] = new Plane(Vec3(0, -.4f, 0), Vec3(0, 1, 0), Color(.3f, .3f, .3f), 0.03f * roughnessMul, 1, 0);
		objects[1] = new Sphere(Vec3(0, 0, -2), .33f, Color(1, 0, 0), 0 * roughnessMul, .25f, 0);
		objects[2] = new Sphere(Vec3(-.25f, .5f, -1.5f), .25f, Color(0, 1, 0), 0 * roughnessMul, 1, 1);
		objects[3] = new Sphere(Vec3(1, -.75f, -2), 1, Color(0, 0, 1), 0 * roughnessMul, 1, 1);
		objects[4] = new Sphere(Vec3(.2f, .25f, -1.5f), .1f, Color(1, 1, 0), 0 * roughnessMul, 1, .5f);

		// init scene lights
		ambientLight = Color(.05f, .05f, .05f);
		pointLightsLength = 1;
		pointLights = new PointLight*[pointLightsLength];
		pointLights[0] = new PointLight(Vec3(.1f, .75f, -2), .01f, Color(1, 1, 1) * 10, 1);

		// start threads
		threadsAlive = true;
		threadCount = Environment::ProcessorCount();
		std::cout << "Thread Count: " << threadCount << std::endl;
		threadDatas = new RenderThreadData*[threadCount];
		threads = new std::thread*[threadCount];
		for (int i = 0; i != threadCount; ++i)
		{
			int sectionHeight = height / threadCount;
			threadDatas[i] = new RenderThreadData();
			threadDatas[i]->exit = false;
			threadDatas[i]->waiting = true;
			threadDatas[i]->done = false;
			threadDatas[i]->index = i,
			threadDatas[i]->randomIndex = i % threadCount,
			threadDatas[i]->x = 0,
			threadDatas[i]->y = i * sectionHeight,
			threadDatas[i]->xEnd = width,
			threadDatas[i]->yEnd = (i * sectionHeight) + sectionHeight;
			if (i == threadCount - 1) threadDatas[i]->yEnd = height;
			threads[i] = new std::thread(RenderThread, threadDatas[i]);
		}
	}

	static void Dispose()
	{
		for (int i = 0; i != threadCount; ++i)
		{
			auto data = threadDatas[i];
			data->exit = true;
			data->done = true;
			data->waiting = false;
		}
	}

	static RawMovie* Render(bool singleThreaded)
	{
		if (singleThreaded)
		{
			// setup ray
			Ray ray;
			ray.origin = camera->position;

			// render frames
			Num widthRange = width - 1;
			Num heightRange = height - 1;
			for (int f = 0; f != frames; ++f)
			{
				std::cout << "Frame:" << f << " rendering" << std::endl;
				UpdateMovieObjects(f);
				auto colors = movie->frames[f]->colors;
				for (int y = 0; y != height; ++y)
				{
					for (int x = 0; x != width; ++x)
					{
						ray.direction = Vec3((x / (Num)(width - 1)) - .5f, (y / (Num)(height - 1)) - .5f, -1).Normalize();
						ray.direction = camera->matrix.Transform(ray.direction);

						// generate shared random vector
						Num randomVal = RandomUtil::NextValue(x / widthRange, y / heightRange);
						auto randomVec = Vec3(randomVal, randomVal, randomVal) - .5f;

						// trace
						int maxBounce = maxBounceCount;
						auto light = Color();
						Trace(ray, light, maxBounce, randomVec);

						// write result
						colors[x + (y * width)] = light;
					}
				}
			}
		}
		else
		{
			// render frames
			for (int f = 0; f != frames; ++f)
			{
				std::cout << "Frame:" << f << " rendering" << std::endl;
				UpdateMovieObjects(f);
				auto frame = movie->frames[f];

				// tell threads to start processing task
				for (int i = 0; i != threadCount; ++i)
				{
					auto data = threadDatas[i];
					data->image = frame;
					data->done = false;
					data->waiting = false;
				}

				// wait for all threads to finish
				for (int i = 0; i != threadCount; ++i)
				{
					auto data = threadDatas[i];
					while (!data->done) Sleep(1);
				}
			}
		}

		return movie;
	}

	static void UpdateMovieObjects(int frame)
	{
		Num rot = frame * .1f;
		for (int l = 0; l != pointLightsLength; ++l)
		{
			auto pointLight = pointLights[l];
			Num r = rot * pointLight->rotSpeed;
			pointLight->position = pointLight->rotPos + (Vec3(MATH::Cos(r), MATH::Sin(r) * 4, MATH::Sin(r) * 3) * .1f);
			pointLight->color = Color(MATH::Abs(MATH::Cos(r)) * 10, MATH::Abs(MATH::Sin(r)) * 10, MATH::Abs(MATH::Cos(r * .33f) * 10));
		}

		Num c = -rot * .25f;
		auto center = Vec3(0, 0, -2);
		camera->position = center + Vec3(MATH::Sin(c), MATH::Sin(c) * .1f, -MATH::Cos(c)) * 2;
		camera->forward = (center - camera->position).Normalize();
		camera->Update();
	}

	static void RenderThread(void* dataObj)
	{
		auto data = (RenderThreadData*)dataObj;

		while (!data->exit)
		{
			// wait for main thread to trigger
			while (data->waiting) Sleep(1);
			if (data->done) return;

			// setup ray
			Ray ray;
			ray.origin = camera->position;

			// render image section
			auto colors = data->image->colors;
			Num widthRange = width - 1;
			Num heightRange = height - 1;
			for (int y = data->y; y < data->yEnd; ++y)
			{
				for (int x = data->x; x < data->xEnd; ++x)
				{
					ray.direction = Vec3((x / (Num)(width - 1)) - .5f, (y / (Num)(height - 1)) - .5f, -1).Normalize();
					ray.direction = camera->matrix.Transform(ray.direction);

					// generate shared random vector
					Num randomVal = RandomUtil::NextValue(x / widthRange, y / heightRange);
					auto randomVec = Vec3(randomVal, randomVal, randomVal) - .5f;

					// trace
					int maxBounce = maxBounceCount;
					auto light = Color();
					Trace(ray, light, maxBounce, randomVec);

					// write result
					colors[x + (y * width)] = light;
				}
			}

			// tell main thread we're done
			data->done = true;
			data->waiting = true;
		}
	}

	static bool Trace(Ray& ray, Color& light, int& maxBounce, const Vec3& randomVec)
	{
		// find closes object
		Obj* closestObject = nullptr;
		Num closestDis = Math::MaxValue();
		auto closestInfo = HitInfo();
		for (int o = 0; o != objectsLength; ++o)
		{
			auto obj = objects[o];
			HitInfo info;
			if (obj->Hit(ray, info))
			{
				Num dis = (ray.origin - info.point).Length();
				if (dis < closestDis)
				{
					closestDis = dis;
					closestObject = obj;
					closestInfo = info;
				}
			}
		}

		// hit point light orb
		for (int l = 0; l != pointLightsLength; ++l)
		{
			auto pointLight = pointLights[l];
			PointLight* closestLight = NULL;
			HitInfo info;
			if (ray.HitSphere(pointLight->position, pointLight->radius, info))
			{
				Num dis = (ray.origin - info.point).Length();
				if (dis < closestDis)
				{
					closestDis = dis;
					closestLight = pointLight;
					closestInfo = info;
				}
			}

			if (closestLight != NULL)
			{
				light = closestLight->color;
				return true;
			}
		}

		// process lighting
		if (closestObject != nullptr)
		{
			if (maxBounce <= 0) return false;
			maxBounce--;
			int maxBounceReset = maxBounce;

			// buffer diffuse pass state objects
			auto reflectedClosestInfo = closestInfo;
			auto reflectedRay = ray;

			// diffuse surface normal
			reflectedClosestInfo.normal = closestInfo.normal + ((randomVec * closestObject->roughness) / (closestDis + 1));
			reflectedRay.direction = ray.direction.Reflect(reflectedClosestInfo.normal.Normalize());
			reflectedRay.origin = closestInfo.point - (reflectedRay.direction * .0001f);// move a little off surface to correct for floating point errors

			// gather surface light
			GatherLight(closestObject, reflectedClosestInfo, light, randomVec);

			// trace from this reflected path
			maxBounce = maxBounceReset;// reset max bounce here or we lose light average consistancy
			auto reflectedLight = Color();
			Trace(reflectedRay, reflectedLight, maxBounce, randomVec);

			// material (reflection)
			Num fresnel = 1 - Vec3::FresnelSchlickZero(ray.direction, closestInfo.normal);
			light += Color::Lerp(reflectedLight, reflectedLight * closestObject->color, closestObject->metalic * fresnel) * closestObject->reflectiveness;

			// volume fog tracing taking into account shadows
			#if !DISABLE_FOG
			auto fogPos = ray.origin;
			auto fogStep = ray.direction * volumeFogStep;
			int maxVolumeFogStepsClamped = (int)MATH::Min(closestDis / volumeFogStep, maxVolumeFogSteps);
			for (int l = 0; l != pointLightsLength; ++l)
			{
				Num fogBrightness = 0;
				auto pointLight = pointLights[l];
				for (int i = 0; i != maxVolumeFogStepsClamped; ++i)
				{
					fogPos += fogStep;
					Num lightDis;
					Ray lightRay;
					if (!IsInShadow(pointLight, fogPos, randomVec, lightDis, lightRay)) fogBrightness += 1 / MATH::Pow(lightDis + 1, 4);
				}
				light += pointLight->color * fogBrightness * volumeFogStep * .1f;
			}
			#endif
				
			return true;
		}
		else
		{
			// volume fog tracing taking into account shadows (optimized for when nothing is hit)
			#if !DISABLE_FOG
			auto fogPos = ray.origin;
			auto fogStep = ray.direction * volumeFogStep;
			for (int l = 0; l != pointLightsLength; ++l)
			{
				Num fogBrightness = 0;
				auto pointLight = pointLights[l];
				for (int i = 0; i != maxVolumeFogSteps; ++i)
				{
					fogPos += fogStep;
					Num lightDis;
					Ray lightRay;
					if (!IsInShadow(pointLight, fogPos, randomVec, lightDis, lightRay)) fogBrightness += 1 / MATH::Pow(lightDis + 1, 4);
				}
				light += pointLight->color * fogBrightness * volumeFogStep * .1f;
			}
			#endif
		}

		// hit nothing
		return false;
	}

	static void GatherLight(Obj* obj, const HitInfo& info, Color& light, const Vec3& randomVec)
	{
		auto objColor = obj->color;

		// ambient
		light += objColor * ambientLight;

		// point
		for (int l = 0; l != pointLightsLength; ++l)
		{
			auto pointLight = pointLights[l];

			// diffuse
			Num lightDis;
			Ray lightRay;
			if (!IsInShadow(pointLights[l], info.point, randomVec, lightDis, lightRay))
			{
				Num d = MATH::Max(info.normal.Dot(lightRay.direction), 0);
				d /= MATH::Pow(lightDis + 1, 4);
				light += objColor * pointLight->color * d;
			}
		}
	}

	static bool IsInShadow(PointLight* pointLight, const Vec3& point, const Vec3& randomVec, Num& lightDis, Ray& lightRay)
	{
		auto softVec = (pointLight->position + (randomVec * pointLight->radius)) - point;
		lightDis = softVec.Length();
		lightRay = Ray(point, softVec.Normalize());
		for (int o = 0; o != objectsLength; ++o)
		{
			auto obj = objects[o];
			HitInfo hitInfo;
			if (obj->Hit(lightRay, hitInfo) && (hitInfo.point - point).Length() <= lightDis)
			{
				return true;
			}
		}
		return false;
	}
};