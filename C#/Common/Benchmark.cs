#define NUM32
//#define NUM64
//#define DISABLE_FOG
//#define VIRTUAL_ON

using System;
using System.Runtime.CompilerServices;
using System.Threading;

#if NUM32
using Num = System.Single;
#elif NUM64
using Num = System.Double;
#endif

namespace SimpleRayBench
{
	static class MATH
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Clamp(Num value, Num min, Num max)
		{
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Sqrt(Num value)
		{
			#if NUM32
			return MathF.Sqrt(value);
			#else
			return Math.Sqrt(value);
			#endif
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Sin(Num value)
		{
			#if NUM32
			return MathF.Sin(value);
			#else
			return Math.Sin(value);
			#endif
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Cos(Num value)
		{
			#if NUM32
			return MathF.Cos(value);
			#else
			return Math.Cos(value);
			#endif
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Floor(Num value)
		{
			#if NUM32
			return MathF.Floor(value);
			#else
			return Math.Floor(value);
			#endif
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Pow(Num value, Num x)
		{
			#if NUM32
			return MathF.Pow(value, x);
			#else
			return Math.Pow(value, x);
			#endif
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Pow4(Num value)
		{
			Num v2 = value * value;
			return v2 * v2;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Pow5(Num value)
		{
			Num v2 = value * value;
			return v2 * v2 * value;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Pow(Num value, int x)
		{
			if (x == 4) return Pow4(value);
			if (x == 5) return Pow5(value);
			return Pow(value, (Num)x);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Max(Num value, Num x)
		{
			return (value > x) ? value : x;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Min(Num value, Num x)
		{
			return (value < x) ? value : x;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num Abs(Num value)
		{
			#if NUM32
			return MathF.Abs(value);
			#else
			return Math.Abs(value);
			#endif
		}
	}

	static class RandomUtil
	{
		[ThreadStatic]
		private static Num offsetX, offsetY;
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num NextValue(Num u, Num v)
		{
			const Num randoValueX = 12.9898f;
			const Num randoValueY = 78.233f;
			const Num randoValueZ = 43758.543123f;

			u += offsetX;
			v += offsetY;
			offsetX += .5f;
			offsetY += .75f;
			
			Num result = (u * randoValueX) + (v * randoValueY);
			result = MATH.Sin(result) * randoValueZ;
			return result - MATH.Floor(result);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num NextValue(Num u, Num v, ref Num ox, ref Num oy)
		{
			const Num randoValueX = 12.9898f;
			const Num randoValueY = 78.233f;
			const Num randoValueZ = 43758.543123f;

			u += ox;
			v += oy;
			ox += .5f;
			oy += .75f;
			
			Num result = (u * randoValueX) + (v * randoValueY);
			result = MATH.Sin(result) * randoValueZ;
			return result - MATH.Floor(result);
		}
	}

	public struct Color
	{
		public Num r, g, b;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public Color(Num r, Num g, Num b)
		{
			this.r = r;
			this.g = g;
			this.b = b;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color operator+(Color p1, Color p2)
		{
			return new Color(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color operator-(Color p1, Color p2)
		{
			return new Color(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color operator*(Color p1, Color p2)
		{
			return new Color(p1.r * p2.r, p1.g * p2.g, p1.b * p2.b);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color operator*(Color p1, Num p2)
		{
			return new Color(p1.r * p2, p1.g * p2, p1.b * p2);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color operator/(Color p1, Num p2)
		{
			return new Color(p1.r / p2, p1.g / p2, p1.b / p2);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color Lerp(Color p1, Color p2, Num t)
		{
			return p1 + ((p2 - p1) * t);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Color LerpClamp(Color p1, Color p2, Num t)
		{
			return p1 + ((p2 - p1) * MATH.Clamp(t, 0, 1));
		}
	}

	struct Vec3
	{
		public Num x, y, z;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public Vec3(Num x, Num y, Num z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator +(Vec3 p1, Vec3 p2)
		{
			return new Vec3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator -(Vec3 p1, Vec3 p2)
		{
			return new Vec3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator-(Vec3 p1, Num p2)
		{
			return new Vec3(p1.x - p2, p1.y - p2, p1.z - p2);
		}
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator-(Vec3 p1)
		{
			return new Vec3(-p1.x, -p1.y, -p1.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator*(Vec3 p1, Vec3 p2)
		{
			return new Vec3(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator*(Vec3 p1, Num p2)
		{
			return new Vec3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator*(Num p1, Vec3 p2)
		{
			return new Vec3(p1 * p2.x, p1 * p2.y, p1 * p2.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator/(Vec3 p1, Vec3 p2)
		{
			return new Vec3(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vec3 operator/(Vec3 p1, Num p2)
		{
			return new Vec3(p1.x / p2, p1.y / p2, p1.z / p2);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Num Dot()
		{
			return (x*x) + (y*y) + (z*z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Num Dot(Vec3 vec)
		{
			return (x*vec.x) + (y*vec.y) + (z*vec.z);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Num Length()
		{
			return MATH.Sqrt((x*x) + (y*y) + (z*z));
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Vec3 Normalize()
		{
			return this / MATH.Sqrt((x*x) + (y*y) + (z*z));
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Vec3 Reflect(Vec3 planeNormal)
		{
			return this - (planeNormal * this.Dot(planeNormal) * 2);
		}
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Vec3 Cross(Vec3 vector)
		{
			return new Vec3(((y*vector.z) - (z*vector.y)), ((z*vector.x) - (x*vector.z)), ((x*vector.y) - (y*vector.x)));
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Num FresnelSchlickZero(Vec3 rayDirection, Vec3 normal)
		{
			return MATH.Pow5(1 - MATH.Max((-rayDirection).Dot(normal), 0));
		}
	}
	
	struct Mat3
	{
		public Vec3 x, y, z;
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly Vec3 Transform(Vec3 v)
		{
			return (x*v.x) + (y*v.y) + (z*v.z);
		}
	}
	
	struct Ray
	{
		public Vec3 origin, direction;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public Ray(Vec3 origin, Vec3 direction)
		{
			this.origin = origin;
			this.direction = direction;
		}
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static bool HitSphere(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, Num sphereRadius, out Vec3 hitPoint, out Vec3 hitNormal)
		{
			Vec3 d = rayOrigin - sphereCenter;
			Num b = d.Dot(rayDirection);

			// if ray pointing wrong direction ignore
			if (b >= 0)
			{
				hitPoint = default;
				hitNormal = default;
				return false;
			}

			// compute
			Num a = rayDirection.Dot(rayDirection);
			Num c = d.Dot() - sphereRadius * sphereRadius;

			// if ray not pointing at object ignore
			Num disc = b * b - a * c;
			if (disc < 0)
			{
				hitPoint = default;
				hitNormal = default;
				return false;
			}

			// compute
			Num sqrtDisc = MATH.Sqrt(disc);
			Num invA = 1 / a;
			Num t1 = (-b - sqrtDisc) * invA;
			
			hitPoint = rayOrigin + t1 * rayDirection;
			hitNormal = (hitPoint - sphereCenter).Normalize();

			return true;
		}
	}

	struct Material
	{
		public Color color;
		public Num roughness;
		public Num reflectiveness, metalic;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public Material(Color color, Num roughness, Num reflectiveness, Num metalic)
		{
			this.color = color;
			this.roughness = roughness;
			this.reflectiveness = reflectiveness;
			this.metalic = metalic;
		}
	}

	enum ObjType
	{
		Plane,
		Sphere
	}

	struct Obj
	{
		public ObjType type;
		public Material material;
		public Vec3 center;
		public Vec3 normal;
		public Num radius;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Obj CreatePlane(Vec3 center, Vec3 normal, Material material)
		{
			return new Obj
			{
				type = ObjType.Plane,
				center = center,
				normal = normal.Normalize(),
				material = material
			};
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Obj CreateSphere(Vec3 center, Num radius, Material material)
		{
			return new Obj
			{
				type = ObjType.Sphere,
				center = center,
				radius = radius,
				material = material
			};
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly bool Hit(Vec3 rayOrigin, Vec3 rayDirection, out Vec3 hitPoint, out Vec3 hitNormal)
		{
			switch (type)
			{
				case ObjType.Sphere: return Ray.HitSphere(rayOrigin, rayDirection, center, radius, out hitPoint, out hitNormal);
				case ObjType.Plane: return HitPlane(rayOrigin, rayDirection, out hitPoint, out hitNormal);
			}

			hitPoint = default;
			hitNormal = default;
			return false;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly bool HitPlane(Vec3 rayOrigin, Vec3 rayDirection, out Vec3 hitPoint, out Vec3 hitNormal)
		{
			Num denom = normal.Dot(rayDirection);
			// if ray pointing wrong direction ignore
			if (denom >= 0)
			{
				hitPoint = default;
				hitNormal = default;
				return false;
			}

			if (MATH.Abs(denom) > 0.0001f)
			{
				Num t = (center - rayOrigin).Dot(normal) / denom;
				if (t >= 0)
				{
					hitPoint = rayOrigin + (rayDirection * t);
					hitNormal = normal;
					return true;
				}
			}

			hitPoint = default;
			hitNormal = default;
			return false;
		}
	}

	public class RawImage
	{
		public int width, height;
		public Color[] colors;

		public RawImage(int width, int height)
		{
			this.width = width;
			this.height = height;
			colors = new Color[width * height];
		}
	}

	public class RawMovie
	{
		public int width, height;
		public RawImage[] frames;

		public RawMovie(int width, int height, int frameCount)
		{
			this.width = width;
			this.height = height;
			frames = new RawImage[frameCount];
			for (int i = 0; i != frames.Length; ++i)
			{
				frames[i] = new RawImage(width, height);
			}
		}
	}

	class Camera
	{
		public Vec3 position, forward, up;
		public Mat3 matrix;

		public Camera(Vec3 position, Vec3 forward, Vec3 up)
		{
			this.position = position;
			this.forward = forward.Normalize();
			this.up = up.Normalize();
			Update();
		}
		
		public void Update()
		{
			var f = -forward;
			var r = up.Cross(f);
			var u = f.Cross(r);
			matrix.x = r.Normalize();
			matrix.y = u.Normalize();
			matrix.z = f.Normalize();
		}
	}
	
	struct PointLight
	{
		public Vec3 position;
		public Num radius;
		public Color color;
		
		public Vec3 rotPos;
		public Num rotSpeed;
		
		public PointLight(Vec3 position, Num radius, Color color, Num rotSpeed)
		{
			this.position = position;
			this.radius = radius;
			this.color = color;
			
			rotPos = position;
			this.rotSpeed = rotSpeed;
		}
	}

	class RenderThreadData
	{
		public int index, randomIndex;
		public volatile bool waiting, done, exit;
		public int x, y, xEnd, yEnd;
		public RawImage image;
		public Num offsetX, offsetY;
	}

	public static class Benchmark
	{
		const int maxBounceCount = 10;// how many ray-bounces are allowed
		const Num roughnessMul = 1;// force tween material value

		const int maxVolumeFogSteps = 250 / 4;// how far can we test the shadow volume for
		const Num volumeFogStep = .01f * 4;// how little the shadow volume testing step is

		private static int width, height, frames;
		private static RawMovie movie;
		private static Camera camera;
		private static Obj[] objects;
		private static Color ambientLight;
		private static PointLight[] pointLights;

		private static RenderThreadData[] threadDatas;
		private static Thread[] threads;

		public static void Init(int width, int height, int frames)
		{
			// init movie
			Benchmark.width = width;
			Benchmark.height = height;
			if (frames < 1) frames = 1;
			Benchmark.frames = frames;
			movie = new RawMovie(width, height, frames);

			// init camera
			camera = new Camera(new Vec3(0, 0, 0), new Vec3(0, 0, -1), new Vec3(0, 1, 0));

			// init scene objects
			objects = new Obj[]
			{
				Obj.CreatePlane(new Vec3(0, -.4f, 0), new Vec3(0, 1, 0), new Material(new Color(.3f, .3f, .3f), 0.03f * roughnessMul, 1, 0)),
				Obj.CreateSphere(new Vec3(0, 0, -2), .33f, new Material(new Color(1, 0, 0), 0 * roughnessMul, .25f, 0)),
				Obj.CreateSphere(new Vec3(-.25f, .5f, -1.5f), .25f, new Material(new Color(0, 1, 0), 0 * roughnessMul, 1, 1)),
				Obj.CreateSphere(new Vec3(1, -.75f, -2), 1, new Material(new Color(0, 0, 1), 0 * roughnessMul, 1, 1)),
				Obj.CreateSphere(new Vec3(.2f, .25f, -1.5f), .1f, new Material(new Color(1, 1, 0), 0 * roughnessMul, 1, .5f))
			};

			// init scene lights
			ambientLight = new Color(.05f, .05f, .05f);
			pointLights = new PointLight[]
			{
				new PointLight(new Vec3(.1f, .75f, -2), .01f, new Color(1, 1, 1) * 10, 1),
				//new PointLight(new Vec3(.1f, .5f, -2.5f), .01f, new Color(1, 0, 0) * 8, .7f),
				//new PointLight(new Vec3(.2f, .75f, -2), .01f, new Color(0, 1, 0) * 8, 1),
				//new PointLight(new Vec3(-.1f, .25f, -1f), .01f, new Color(0, 0, 1) * 8, .5f)
			};

			// start threads
			int threadCount = Environment.ProcessorCount;
			Console.WriteLine("Thread Count: " + threadCount.ToString());
			threadDatas = new RenderThreadData[threadCount];
			threads = new Thread[threadCount];
			for (int i = 0; i != threadCount; ++i)
			{
				threads[i] = new Thread(RenderThread);
				int sectionHeight = height / threadCount;
				threadDatas[i] = new RenderThreadData()
				{
					index = i,
					randomIndex = i % threadCount,
					exit = false,
					waiting = true,
					done = false,
					x = 0,
					y = i * sectionHeight,
					xEnd = width,
					yEnd = (i * sectionHeight) + sectionHeight
				};
				if (i == threadCount - 1) threadDatas[i].yEnd = height;
				threads[i].Start(threadDatas[i]);
			}
		}

		public static void Dispose()
		{
			// tell threads to stop waiting
			foreach (var data in threadDatas)
			{
				data.exit = true;
				data.done = true;
				data.waiting = false;
			}
		}

		public static RawMovie Render(bool singleThreaded)
		{
			if (singleThreaded)
			{
				// render frames
				Num widthRange = width - 1;
				Num heightRange = height - 1;
				for (int f = 0; f != frames; ++f)
				{
					Console.WriteLine($"Frame:{f} rendering");
					UpdateMovieObjects(f);
					Ray ray;
					ray.origin = camera.position;
					var colors = movie.frames[f].colors;
					for (int y = 0; y != height; ++y)
					{
						for (int x = 0; x != width; ++x)
						{
							ray.direction = new Vec3((x / (Num)(width - 1)) - .5f, (y / (Num)(height - 1)) - .5f, -1).Normalize();
							ray.direction = camera.matrix.Transform(ray.direction);

							// generate shared random vector
							Num randomVal = RandomUtil.NextValue(x / widthRange, y / heightRange);
							var randomVec = new Vec3(randomVal, randomVal, randomVal) - .5f;

							// trace
							int maxBounce = maxBounceCount;
							var light = new Color();
							Trace(in ray, ref light, ref maxBounce, randomVec);

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
					Console.WriteLine($"Frame:{f} rendering");
					UpdateMovieObjects(f);
					var frame = movie.frames[f];

					// tell threads to start processing task
					foreach (var data in threadDatas)
					{
						data.image = frame;
						data.done = false;
						data.waiting = false;
					}

					// wait for all threads to finish
					var spin = new SpinWait();
					foreach (var data in threadDatas)
					{
						while (!data.done) spin.SpinOnce(-1);
					}
				}
			}

			return movie;
		}

		private static void UpdateMovieObjects(int frame)
		{
			Num rot = frame * .1f;
			for (int l = 0; l != pointLights.Length; ++l)
			{
				ref var pointLight = ref pointLights[l];
				Num r = rot * pointLight.rotSpeed;
				pointLight.position = pointLight.rotPos + (new Vec3(MATH.Cos(r), MATH.Sin(r) * 4, MATH.Sin(r) * 3) * .1f);
				pointLight.color = new Color(MATH.Abs(MATH.Cos(r)) * 10, MATH.Abs(MATH.Sin(r)) * 10, MATH.Abs(MATH.Cos(r * .33f) * 10));
			}
			
			Num c = -rot * .25f;
			var center = new Vec3(0, 0, -2);
			camera.position = center + new Vec3(MATH.Sin(c), MATH.Sin(c) * .1f, -MATH.Cos(c)) * 2;
			camera.forward = (center - camera.position).Normalize();
			camera.Update();
		}

		[MethodImpl(MethodImplOptions.AggressiveOptimization)]
		private static void RenderThread(object dataObj)
		{
			var data = (RenderThreadData)dataObj;
			var spin = new SpinWait();

			while (!data.exit)
			{
				// wait for main thread to trigger
				while (data.waiting) spin.SpinOnce(-1);
				if (data.done) return;

				// setup ray
				Vec3 rayOrigin = camera.position;
				Mat3 camMatrix = camera.matrix;

				// render image section
				var colors = data.image.colors;
				var objs = objects;
				var lights = pointLights;
				Color ambient = ambientLight;
				int w = width;
				Num widthRange = w - 1;
				Num heightRange = height - 1;
				int xStart = data.x;
				int xEnd = data.xEnd;
				int yEnd = data.yEnd;
				Num ox = data.offsetX;
				Num oy = data.offsetY;
				for (int y = data.y; y < yEnd; ++y)
				{
					int rowOffset = y * w;
					Num yNorm = y / heightRange;
					for (int x = xStart; x < xEnd; ++x)
					{
						Num xNorm = x / widthRange;
						Vec3 rayDirection = new Vec3(xNorm - .5f, yNorm - .5f, -1).Normalize();
						rayDirection = camMatrix.Transform(rayDirection);

						// generate shared random vector
						Num randomVal = RandomUtil.NextValue(xNorm, yNorm, ref ox, ref oy);
						var randomVec = new Vec3(randomVal, randomVal, randomVal) - .5f;

						// trace
						int maxBounce = maxBounceCount;
						var light = new Color();
						Trace(rayOrigin, rayDirection, ref light, ref maxBounce, randomVec, objs, lights, ambient);

						// write result
						colors[x + rowOffset] = light;
					}
				}
				data.offsetX = ox;
				data.offsetY = oy;

				// tell main thread we're done
				data.done = true;
				data.waiting = true;
			}
		}

		[MethodImpl(MethodImplOptions.AggressiveOptimization)]
		static bool Trace(in Ray ray, ref Color light, ref int maxBounce, Vec3 randomVec)
		{
			return Trace(ray.origin, ray.direction, ref light, ref maxBounce, randomVec, objects, pointLights, ambientLight);
		}

		[MethodImpl(MethodImplOptions.AggressiveOptimization)]
		static bool Trace(Vec3 rayOrigin, Vec3 rayDirection, ref Color light, ref int maxBounce, Vec3 randomVec, Obj[] objs, PointLight[] lights, Color ambient)
		{
			// find closes object
			int closestObjectIdx = -1;
			Num closestDis = Num.MaxValue;
			Vec3 closestPoint = default;
			Vec3 closestNormal = default;
			for (int o = 0; o < objs.Length; ++o)
			{
				if (objs[o].Hit(rayOrigin, rayDirection, out var infoPoint, out var infoNormal))
				{
					Num dis = (rayOrigin - infoPoint).Length();
					if (dis < closestDis)
					{
						closestDis = dis;
						closestObjectIdx = o;
						closestPoint = infoPoint;
						closestNormal = infoNormal;
					}
				}
			}
			
			// hit point light orb
			for (int l = 0; l < lights.Length; ++l)
			{
				ref readonly var pointLight = ref lights[l];
				bool hitLight = false;
				if (Ray.HitSphere(rayOrigin, rayDirection, pointLight.position, pointLight.radius, out var infoPoint, out var infoNormal))
				{
					Num dis = (rayOrigin - infoPoint).Length();
					if (dis < closestDis)
					{
						closestDis = dis;
						hitLight = true;
						closestPoint = infoPoint;
						closestNormal = infoNormal;
					}
				}
				
				if (hitLight)
				{
					light = pointLight.color;
					return true;
				}
			}

			// process lighting
			if (closestObjectIdx >= 0)
			{
				if (maxBounce <= 0) return false;
				maxBounce--;
				int maxBounceReset = maxBounce;

				// buffer diffuse pass state objects
				ref readonly var closestObject = ref objs[closestObjectIdx];
				ref readonly var material = ref closestObject.material;

				// diffuse surface normal
				Vec3 reflectedNormal = closestNormal + ((randomVec * material.roughness) / (closestDis + 1));
				Vec3 reflectedDirection = rayDirection.Reflect(reflectedNormal.Normalize());
				Vec3 reflectedOrigin = closestPoint - (reflectedDirection * .0001f);// move a little off surface to correct for floating point errors

				// gather surface light
				GatherLight(in closestObject, closestPoint, reflectedNormal, ref light, randomVec, objs, lights, ambient);

				// trace from this reflected path
				maxBounce = maxBounceReset;// reset max bounce here or we lose light average consistancy
				var reflectedLight = new Color();
				Trace(reflectedOrigin, reflectedDirection, ref reflectedLight, ref maxBounce, randomVec, objs, lights, ambient);

				// material (reflection)
				Num fresnel = 1 - Vec3.FresnelSchlickZero(rayDirection, closestNormal);
				light += Color.Lerp(reflectedLight, reflectedLight * material.color, material.metalic * fresnel) * material.reflectiveness;
				
				// volume fog tracing taking into account shadows
				#if !DISABLE_FOG
				var fogPos = rayOrigin;
				var fogStep = rayDirection * volumeFogStep;
				int maxVolumeFogStepsClamped = (int)MATH.Min(closestDis / volumeFogStep, maxVolumeFogSteps);
				for (int l = 0; l < lights.Length; ++l)
				{
					Num fogBrightness = 0;
					ref readonly var pointLight = ref lights[l];
					for (int i = 0; i < maxVolumeFogStepsClamped; ++i)
					{
						fogPos += fogStep;
						if (!IsInShadow(in pointLight, fogPos, randomVec, objs, out var lightDis, out _)) fogBrightness += 1 / MATH.Pow4(lightDis + 1);
					}
					light += pointLight.color * fogBrightness * volumeFogStep * .1f;
				}
				#endif
				
				return true;
			}
			else
			{
				// volume fog tracing taking into account shadows (optimized for when nothing is hit)
				#if !DISABLE_FOG
				var fogPos = rayOrigin;
				var fogStep = rayDirection * volumeFogStep;
				for (int l = 0; l < lights.Length; ++l)
				{
					Num fogBrightness = 0;
					ref readonly var pointLight = ref lights[l];
					for (int i = 0; i < maxVolumeFogSteps; ++i)
					{
						fogPos += fogStep;
						if (!IsInShadow(in pointLight, fogPos, randomVec, objs, out var lightDis, out _)) fogBrightness += 1 / MATH.Pow4(lightDis + 1);
					}
					light += pointLight.color * fogBrightness * volumeFogStep * .1f;
				}
				#endif
			}

			// hit nothing
			return false;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		static void GatherLight(in Obj obj, Vec3 infoPoint, Vec3 infoNormal, ref Color light, Vec3 randomVec, Obj[] objs, PointLight[] lights, Color ambient)
		{
			var objColor = obj.material.color;

			// ambient
			light += objColor * ambient;
			
			// point
			for (int l = 0; l < lights.Length; ++l)
			{
				ref readonly var pointLight = ref lights[l];
				
				// diffuse
				if (!IsInShadow(in pointLight, infoPoint, randomVec, objs, out var lightDis, out var lightDir))
				{
					Num d = MATH.Max(infoNormal.Dot(lightDir), 0);
					d /= MATH.Pow4(lightDis + 1);
					light += objColor * pointLight.color * d;
				}
			}
		}
		
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		static bool IsInShadow(in PointLight pointLight, Vec3 point, Vec3 randomVec, Obj[] objs, out Num lightDis, out Vec3 lightDir)
		{
			var softVec = (pointLight.position + (randomVec * pointLight.radius)) - point;
			lightDis = softVec.Length();
			lightDir = softVec.Normalize();
			for (int o = 0; o < objs.Length; ++o)
			{
				if (objs[o].Hit(point, lightDir, out var hitPoint, out _) && (hitPoint - point).Length() <= lightDis)
				{
					return true;
				}
			}
			return false;
		}
	}
}