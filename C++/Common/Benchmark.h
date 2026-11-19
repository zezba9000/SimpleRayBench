#pragma once
#include "LangUtils.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <system_error>
#include <vector>

#define NUM32
//#define NUM64
//#define DISABLE_FOG
//#define VIRTUAL_ON

#if defined(NUM32)
#define Num float
#define MATH MathF
#elif defined(NUM64)
#define Num double
#define MATH Math
#endif

namespace SimpleRayBench
{
	class RandomUtil
	{
	private:
		Num offsetX, offsetY;

	public:
		RandomUtil(int frame) : offsetX(frame * .5f), offsetY(frame * .75f)
		{ }

		inline
		Num NextValue(Num u, Num v)
		{
			#define randoValueX 12.9898f
			#define randoValueY 78.233f
			#define randoValueZ 43758.543123f

			u += offsetX;
			v += offsetY;
			offsetX += .5f;
			offsetY += .75f;

			Num result = (u * randoValueX) + (v * randoValueY);
			result = MATH::Sin(result) * randoValueZ;
			return result - MATH::Floor(result);
		}
	};

	struct Color
	{
	public:
		Num r, g, b;

		Color()
		{
			r = 0;
			g = 0;
			b = 0;
		}

		Color(Num r, Num g, Num b)
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}

		inline
		Color operator+(const Color p2) const
		{
			return Color(r + p2.r, g + p2.g, b + p2.b);
		}

		inline
		void operator+=(const Color p1)
		{
			this->r += p1.r;
			this->g += p1.g;
			this->b += p1.b;
		}

		inline
		Color operator-(const Color p2) const
		{
			return Color(r - p2.r, g - p2.g, b - p2.b);
		}

		inline
		Color operator*(const Color p2) const
		{
			return Color(r * p2.r, g * p2.g, b * p2.b);
		}

		inline
		friend Color operator*(const Color p1, Num p2)
		{
			return Color(p1.r * p2, p1.g * p2, p1.b * p2);
		}

		inline
		friend Color operator/(const Color p1, Num p2)
		{
			return Color(p1.r / p2, p1.g / p2, p1.b / p2);
		}

		inline
		static Color Lerp(Color p1, Color p2, Num t)
		{
			return p1 + ((p2 - p1) * t);
		}

		inline
		static Color LerpClamp(Color p1, Color p2, Num t)
		{
			return p1 + ((p2 - p1) * MATH::Clamp(t, 0, 1));
		}
	};

	struct Vec3
	{
	public:
		Num x, y, z;

		Vec3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vec3(Num x, Num y, Num z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline
		Vec3 operator +(const Vec3 p2) const
		{
			return Vec3(x + p2.x, y + p2.y, z + p2.z);
		}

		inline
		void operator+=(const Vec3 p1)
		{
			this->x += p1.x;
			this->y += p1.y;
			this->z += p1.z;
		}

		inline
		Vec3 operator -(const Vec3 p2) const
		{
			return Vec3(x - p2.x, y - p2.y, z - p2.z);
		}

		inline
		friend Vec3 operator-(const Vec3 p1, Num p2)
		{
			return Vec3(p1.x - p2, p1.y - p2, p1.z - p2);
		}

		inline
		Vec3 operator-() const
		{
			return Vec3(-x, -y, -z);
		}

		inline
		Vec3 operator*(const Vec3 p2) const
		{
			return Vec3(x * p2.x, y * p2.y, z * p2.z);
		}

		inline
		friend Vec3 operator*(const Vec3 p1, Num p2)
		{
			return Vec3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		inline
		friend Vec3 operator*(Num p1, const Vec3 p2)
		{
			return Vec3(p1 * p2.x, p1 * p2.y, p1 * p2.z);
		}

		inline
		Vec3 operator/(const Vec3 p2) const
		{
			return Vec3(x / p2.x, y / p2.y, z / p2.z);
		}

		inline
		friend Vec3 operator/(const Vec3 p1, Num p2)
		{
			return Vec3(p1.x / p2, p1.y / p2, p1.z / p2);
		}

		inline
		Num Dot() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		inline
		Num Dot(Vec3 vec) const
		{
			return (x * vec.x) + (y * vec.y) + (z * vec.z);
		}

		inline
		Num Length() const
		{
			return MATH::Sqrt((x * x) + (y * y) + (z * z));
		}

		inline
		Vec3 Normalize() const
		{
			return (*this) / MATH::Sqrt((x * x) + (y * y) + (z * z));
		}

		inline
		Vec3 Reflect(Vec3 planeNormal) const
		{
			return (*this) - (planeNormal * this->Dot(planeNormal) * 2);
		}

		inline
		Vec3 Cross(Vec3 vector) const
		{
			return Vec3(((y*vector.z) - (z*vector.y)), ((z*vector.x) - (x*vector.z)), ((x*vector.y) - (y*vector.x)));
		}

		inline
		static Num FresnelSchlickZero(Vec3 rayDirection, Vec3 normal)
		{
			Num value = 1 - MATH::Max((-rayDirection).Dot(normal), 0);
			Num squared = value * value;
			return squared * squared * value;
		}
	};

	struct Mat3
	{
	public:
		Vec3 x, y, z;

		Vec3 Transform(Vec3 v)
		{
			return (x*v.x) + (y*v.y) + (z*v.z);
		}
	};

	struct HitInfo
	{
	public:
		Vec3 point, normal;

		HitInfo()
		{
			point = Vec3();
			normal = Vec3();
		}
	};

	struct Ray
	{
	public:
		Vec3 origin, direction;

		Ray()
		{
			origin = Vec3();
			direction = Vec3();
		}

		Ray(Vec3 origin, Vec3 direction)
		{
			this->origin = origin;
			this->direction = direction;
		}

		bool HitSphereNumerator(Vec3 sphereCenter, Num sphereRadius, Num a, Num& numerator) const
		{
			Vec3 d = origin - sphereCenter;
			Num b = d.Dot(direction);
			if (b >= 0) return false;

			Num c = d.Dot() - sphereRadius * sphereRadius;
			Num disc = b * b - a * c;
			if (disc < 0) return false;

			numerator = -b - MATH::Sqrt(disc);
			return true;
		}

		bool HitSphere(Vec3 sphereCenter, Num sphereRadius, HitInfo& info) const
		{
			Num a = direction.Dot();
			Num numerator;
			if (!HitSphereNumerator(sphereCenter, sphereRadius, a, numerator))
			{
				info = HitInfo();
				return false;
			}

			Num t = numerator * (1 / a);
			info.point = origin + t * direction;
			info.normal = (info.point - sphereCenter).Normalize();

			return true;
		}
	};

	struct Material
	{
	public:
		Color color;
		Num roughness;
		Num reflectiveness, metalic;

		Material()
		{
			color = Color();
			roughness = 0;
			reflectiveness = 0;
			metalic = 0;
		}

		Material(Color color, Num roughness, Num reflectiveness, Num metalic)
		{
			this->color = color;
			this->roughness = roughness;
			this->reflectiveness = reflectiveness;
			this->metalic = metalic;
		}
	};

	enum ObjType
	{
		ObjType_Plane,
		ObjType_Sphere
	};

	class Obj
	{
	public:
		ObjType type;
		Material material;

		Obj(ObjType type)
		{
			material = Material();
			this->type = type;
		}

		#ifdef VIRTUAL_ON
		virtual bool Hit(const Ray& ray, HitInfo& info) const = 0;
		virtual bool BlocksLight(const Ray& ray, Num a) const = 0;
		#else
		bool Hit(const Ray& ray, HitInfo& info) const;
		bool BlocksLight(const Ray& ray, Num a) const;
		#endif
	};

	class Sphere final : public Obj
	{
	public:
		Vec3 center;
		Num radius;

		Sphere(Vec3 center, float radius, Material material) : Obj(ObjType_Sphere)
		{
			type = ObjType_Sphere;
			this->center = center;
			this->radius = radius;
			this->material = material;
		}

		#ifdef VIRTUAL_ON
		bool Hit(const Ray& ray, HitInfo& info) const override
		#else
		inline
		bool HitSphere(const Ray& ray, HitInfo& info) const
		#endif
		{
			return ray.HitSphere(center, radius, info);
		}

		#ifdef VIRTUAL_ON
		bool BlocksLight(const Ray& ray, Num a) const override
		#else
		bool BlocksLight(const Ray& ray, Num a) const
		#endif
		{
			Num numerator;
			// The hit parameter is numerator / a.
			return ray.HitSphereNumerator(center, radius, a, numerator) && MATH::Abs(numerator) <= a;
		}
	};

	class Plane final : public Obj
	{
		static constexpr Num parallelThreshold = .0001f;

	public:
		Vec3 center, normal;

		Plane(Vec3 center, Vec3 normal, Material material) : Obj(ObjType_Plane)
		{
			type = ObjType_Plane;
			this->center = center;
			this->normal = normal.Normalize();
			this->material = material;
		}

		#ifdef VIRTUAL_ON
		bool Hit(const Ray& ray, HitInfo& info) const override
		#else
		inline
		bool HitPlane(const Ray& ray, HitInfo& info) const
		#endif
		{
			Num denom = normal.Dot(ray.direction);
			if (denom < -parallelThreshold)
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

		#ifdef VIRTUAL_ON
		bool BlocksLight(const Ray& ray, Num a) const override
		#else
		bool BlocksLight(const Ray& ray, Num a) const
		#endif
		{
			Num denom = normal.Dot(ray.direction);
			if (denom >= 0 || denom * denom <= a * (parallelThreshold * parallelThreshold)) return false;

			Num offset = (center - ray.origin).Dot(normal);
			return offset <= 0 && offset >= denom;
		}
	};

	#ifndef VIRTUAL_ON
	inline
	bool Obj::Hit(const Ray& ray, HitInfo& info) const
	{
		switch (type)
		{
			case ObjType_Sphere: return static_cast<const Sphere*>(this)->HitSphere(ray, info);
			case ObjType_Plane: return static_cast<const Plane*>(this)->HitPlane(ray, info);
		}

		info = HitInfo();
		return false;
	}

	inline
	bool Obj::BlocksLight(const Ray& ray, Num a) const
	{
		switch (type)
		{
			case ObjType_Sphere: return static_cast<const Sphere*>(this)->BlocksLight(ray, a);
			case ObjType_Plane: return static_cast<const Plane*>(this)->BlocksLight(ray, a);
		}
		return false;
	}
	#endif

	class RawImage
	{
	public:
		int width, height;
		std::vector<Color> colors;

		RawImage(int width, int height) : colors((std::size_t)width * height)
		{
			this->width = width;
			this->height = height;
		}
	};

	class RawMovie
	{
	public:
		int width, height;
		int frameCount;
		std::vector<RawImage> frames;

		RawMovie(int width, int height, int frameCount)
		{
			this->width = width;
			this->height = height;
			this->frameCount = frameCount;
			frames.reserve(frameCount);
			for (int i = 0; i != frameCount; ++i)
			{
				frames.emplace_back(width, height);
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

		enum { objectsLength = 5, pointLightsLength = 1 };
		static int width, height, frames;
		static std::unique_ptr<RawMovie> movie;
		static const Obj* objects[objectsLength];
		static Color ambientLight;

		static int threadCount;

		static void Init(int width, int height, int frames)
		{
			// init movie
			Benchmark::width = width;
			Benchmark::height = height;
			if (frames < 1) frames = 1;
			Benchmark::frames = frames;
			movie.reset(new RawMovie(width, height, frames));

			// init scene objects
			static const Plane plane(Vec3(0, -.4f, 0), Vec3(0, 1, 0), Material(Color(.3f, .3f, .3f), 0.03f * roughnessMul, 1, 0));
			static const Sphere spheres[objectsLength - 1] =
			{
				Sphere(Vec3(0, 0, -2), .33f, Material(Color(1, 0, 0), 0 * roughnessMul, .25f, 0)),
				Sphere(Vec3(-.25f, .5f, -1.5f), .25f, Material(Color(0, 1, 0), 0 * roughnessMul, 1, 1)),
				Sphere(Vec3(1, -.75f, -2), 1, Material(Color(0, 0, 1), 0 * roughnessMul, 1, 1)),
				Sphere(Vec3(.2f, .25f, -1.5f), .1f, Material(Color(1, 1, 0), 0 * roughnessMul, 1, .5f))
			};
			objects[0] = &plane;
			for (int o = 1; o != objectsLength; ++o) objects[o] = &spheres[o - 1];

			ambientLight = Color(.05f, .05f, .05f);

			// choose the number of render threads
			threadCount = Environment::ProcessorCount();
			if (threadCount < 1) threadCount = 1;
			if (threadCount > frames) threadCount = frames;
			std::cout << "Thread Count: " << threadCount << std::endl;
		}

		static void Dispose()
		{
			movie.reset();
		}

		static RawMovie* Render(bool singleThreaded)
		{
			int workerCount = singleThreaded ? 1 : threadCount;
			std::vector<std::thread> threads;
			threads.reserve(workerCount - 1);

			// each thread renders complete frames, interleaved across the movie
			int i = 1;
			try
			{
				for (; i < workerCount; ++i) threads.emplace_back(RenderThread, i, workerCount);
			}
			catch (const std::system_error&)
			{
				// The calling thread renders any ranges whose worker could not start.
			}
			for (; i < workerCount; ++i) RenderThread(i, workerCount);
			RenderThread(0, workerCount);
			for (auto& thread : threads) thread.join();

			return movie.get();
		}

		static void UpdateMovieObjects(Camera& camera, PointLight* pointLights, int frame)
		{
			Num rot = frame * .1f;
			for (int l = 0; l != pointLightsLength; ++l)
			{
				auto& pointLight = pointLights[l];
				Num r = rot * pointLight.rotSpeed;
				pointLight.position = pointLight.rotPos + (Vec3(MATH::Cos(r), MATH::Sin(r) * 4, MATH::Sin(r) * 3) * .1f);
				pointLight.color = Color(MATH::Abs(MATH::Cos(r)) * 10, MATH::Abs(MATH::Sin(r)) * 10, MATH::Abs(MATH::Cos(r * .33f) * 10));
			}

			Num c = -rot * .25f;
			auto center = Vec3(0, 0, -2);
			camera.position = center + Vec3(MATH::Sin(c), MATH::Sin(c) * .1f, -MATH::Cos(c)) * 2;
			camera.forward = (center - camera.position).Normalize();
			camera.Update();
		}

		static void RenderThread(int index, int step)
		{
			Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0));
			PointLight pointLights[pointLightsLength] =
			{
				PointLight(Vec3(.1f, .75f, -2), .01f, Color(1, 1, 1) * 10, 1)
			};
			Num widthRange = (Num)(width - 1);
			Num heightRange = (Num)(height - 1);

			for (int f = index; f < frames; f += step)
			{
				std::printf("Frame:%d rendering\n", f);
				std::fflush(stdout);
				UpdateMovieObjects(camera, pointLights, f);

				// each frame has its own random sequence regardless of which thread renders it
				RandomUtil random(f);
				Ray ray;
				ray.origin = camera.position;
				auto colors = movie->frames[f].colors.data();
				for (int y = 0; y < height; ++y)
				{
					Num v = y / heightRange;
					for (int x = 0; x < width; ++x)
					{
						Num u = x / widthRange;
						ray.direction = Vec3(u - .5f, v - .5f, -1).Normalize();
						ray.direction = camera.matrix.Transform(ray.direction);

						// generate random vector
						Num randomVal = random.NextValue(u, v) - .5f;
						auto randomVec = Vec3(randomVal, randomVal, randomVal);

						// trace
						int maxBounce = maxBounceCount;
						auto light = Color();
						Trace(ray, light, maxBounce, randomVec, pointLights);

						// write result
						*colors++ = light;
					}
				}
			}
		}

		static Num LightFalloff(Num lightDis)
		{
			Num value = lightDis + 1;
			Num squared = value * value;
			return squared * squared;
		}

		#if !DISABLE_FOG
		static void GatherFog(const Ray& ray, Color& light, const Vec3& randomVec, int steps, const PointLight* pointLights)
		{
			auto fogStep = ray.direction * volumeFogStep;
			for (int l = 0; l != pointLightsLength; ++l)
			{
				auto fogPos = ray.origin;
				Num fogBrightness = 0;
				const auto& pointLight = pointLights[l];
				auto lightPosition = pointLight.position + (randomVec * pointLight.radius);
				for (int i = 0; i != steps; ++i)
				{
					fogPos += fogStep;
					Num lightDis;
					if (!IsInShadow(lightPosition, fogPos, lightDis)) fogBrightness += 1 / LightFalloff(lightDis);
				}
				light += pointLight.color * fogBrightness * volumeFogStep * .1f;
			}
		}
		#endif

		static bool Trace(const Ray& ray, Color& light, int& maxBounce, const Vec3& randomVec, const PointLight* pointLights)
		{
			// find closes object
			const Obj* closestObject = nullptr;
			Num closestDis = MATH::MaxValue();
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
				const auto& pointLight = pointLights[l];
				HitInfo info;
				if (ray.HitSphere(pointLight.position, pointLight.radius, info) && (ray.origin - info.point).Length() < closestDis)
				{
					light = pointLight.color;
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
				auto material = &closestObject->material;

				// diffuse surface normal
				reflectedClosestInfo.normal = closestInfo.normal + ((randomVec * material->roughness) / (closestDis + 1));
				reflectedRay.direction = ray.direction.Reflect(reflectedClosestInfo.normal.Normalize());
				reflectedRay.origin = closestInfo.point - (reflectedRay.direction * .0001f);// move a little off surface to correct for floating point errors

				// gather surface light
				GatherLight(closestObject, reflectedClosestInfo, light, randomVec, pointLights);

				// trace from this reflected path
				maxBounce = maxBounceReset;// reset max bounce here or we lose light average consistancy
				auto reflectedLight = Color();
				Trace(reflectedRay, reflectedLight, maxBounce, randomVec, pointLights);

				// material (reflection)
				Num fresnel = 1 - Vec3::FresnelSchlickZero(ray.direction, closestInfo.normal);
				light += Color::Lerp(reflectedLight, reflectedLight * material->color, material->metalic * fresnel) * material->reflectiveness;

				// volume fog tracing taking into account shadows
				#if !DISABLE_FOG
				int maxVolumeFogStepsClamped = (int)MATH::Min(closestDis / volumeFogStep, maxVolumeFogSteps);
				GatherFog(ray, light, randomVec, maxVolumeFogStepsClamped, pointLights);
				#endif
				
				return true;
			}
			else
			{
				// volume fog tracing taking into account shadows (optimized for when nothing is hit)
				#if !DISABLE_FOG
				GatherFog(ray, light, randomVec, maxVolumeFogSteps, pointLights);
				#endif
			}

			// hit nothing
			return false;
		}

		static void GatherLight(const Obj* obj, const HitInfo& info, Color& light, const Vec3& randomVec, const PointLight* pointLights)
		{
			auto objColor = obj->material.color;

			// ambient
			light += objColor * ambientLight;

			// point
			for (int l = 0; l != pointLightsLength; ++l)
			{
				const auto& pointLight = pointLights[l];

				// diffuse
				auto lightPosition = pointLight.position + (randomVec * pointLight.radius);
				Num lightDis;
				Vec3 lightDirection;
				if (!IsInShadow(lightPosition, info.point, lightDis, &lightDirection))
				{
					Num d = MATH::Max(info.normal.Dot(lightDirection), 0);
					d /= LightFalloff(lightDis);
					light += objColor * pointLight.color * d;
				}
			}
		}

		static bool IsInShadow(const Vec3& lightPosition, const Vec3& point, Num& lightDis, Vec3* lightDirection = nullptr)
		{
			// A ray parameter of 1 is the light position.
			Ray lightRay(point, lightPosition - point);
			Num a = lightRay.direction.Dot();
			for (int o = 0; o != objectsLength; ++o)
			{
				if (objects[o]->BlocksLight(lightRay, a)) return true;
			}

			lightDis = MATH::Sqrt(a);
			if (lightDirection != nullptr) *lightDirection = lightRay.direction / lightDis;
			return false;
		}
	};

	Color Benchmark::fogColor = Color(.25f, .25f, .25f);

	int Benchmark::width;
	int Benchmark::height;
	int Benchmark::frames;
	std::unique_ptr<RawMovie> Benchmark::movie;
	const Obj* Benchmark::objects[objectsLength];
	Color Benchmark::ambientLight;

	int Benchmark::threadCount;
}