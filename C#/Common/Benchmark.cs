#define NUM32
//#define NUM64
//#define DISABLE_FOG
//#define VIRTUAL_ON

using System;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;

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
	
	static class Ray
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static bool HitSphereNumerator(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, Num radiusSquared, Num a, out Num numerator)
		{
			Vec3 d = rayOrigin - sphereCenter;
			Num b = d.Dot(rayDirection);
			numerator = default;
			if (b >= 0) return false;

			Num c = d.Dot() - radiusSquared;
			Num disc = b * b - a * c;
			if (disc < 0) return false;

			numerator = -b - MATH.Sqrt(disc);
			return true;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static bool HitSphere(Vec3 rayOrigin, Vec3 rayDirection, Vec3 sphereCenter, Num radiusSquared, Num a, out Vec3 hitPoint, out Vec3 hitNormal)
		{
			if (!HitSphereNumerator(rayOrigin, rayDirection, sphereCenter, radiusSquared, a, out var numerator))
			{
				hitPoint = default;
				hitNormal = default;
				return false;
			}

			Num t = numerator * (1 / a);
			hitPoint = rayOrigin + t * rayDirection;
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
		const Num parallelThreshold = .0001f;

		public ObjType type;
		public Material material;
		public Vec3 center;
		public Vec3 normal;
		private Num radiusSquared;

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
				radiusSquared = radius * radius,
				material = material
			};
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly bool Hit(Vec3 rayOrigin, Vec3 rayDirection, Num a, out Vec3 hitPoint, out Vec3 hitNormal)
		{
			switch (type)
			{
				case ObjType.Sphere: return Ray.HitSphere(rayOrigin, rayDirection, center, radiusSquared, a, out hitPoint, out hitNormal);
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
			if (denom < -parallelThreshold)
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

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public readonly bool BlocksLight(Vec3 point, Vec3 direction, Num a)
		{
			switch (type)
			{
				case ObjType.Sphere:
					// The hit parameter is numerator / a.
					return Ray.HitSphereNumerator(point, direction, center, radiusSquared, a, out var numerator) && MATH.Abs(numerator) <= a;
				case ObjType.Plane:
					Num denom = normal.Dot(direction);
					if (denom >= 0 || denom * denom <= a * (parallelThreshold * parallelThreshold)) return false;

					Num offset = (center - point).Dot(normal);
					return offset <= 0 && offset >= denom;
			}
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

	public static class Benchmark
	{
		const int maxBounceCount = 10;// how many ray-bounces are allowed
		const Num roughnessMul = 1;// force tween material value

		const int maxVolumeFogSteps = 250 / 4;// how far can we test the shadow volume for
		const Num volumeFogStep = .01f * 4;// how little the shadow volume testing step is

		private static RawMovie movie;
		private static Obj[] objects;
		private static Color ambientLight;
		private static PointLight[] pointLights;

		public static void Init(int width, int height, int frames)
		{
			// init movie
			if (frames < 1) frames = 1;
			movie = new RawMovie(width, height, frames);

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

			int threadCount = Math.Min(Environment.ProcessorCount, frames);
			Console.WriteLine("Thread Count: " + threadCount.ToString());
		}

		public static void Dispose()
		{
			movie = null;
		}

		public static RawMovie Render(bool singleThreaded)
		{
			var renderMovie = movie;
			var objs = objects;
			var lights = pointLights;
			Color ambient = ambientLight;
			int workerCount = singleThreaded ? 1 : Math.Min(Environment.ProcessorCount, renderMovie.frames.Length);
			int nextFrame = -1;
			if (workerCount == 1)
			{
				RenderFrames(renderMovie, objs, lights, ambient, ref nextFrame);
			}
			else
			{
				var options = new ParallelOptions { MaxDegreeOfParallelism = workerCount };
				Parallel.For(0, workerCount, options, _ => RenderFrames(renderMovie, objs, lights, ambient, ref nextFrame));
			}
			return renderMovie;
		}

		private static void UpdateMovieObjects(Camera camera, PointLight[] lights, int frame)
		{
			Num rot = frame * .1f;
			for (int l = 0; l != lights.Length; ++l)
			{
				ref var pointLight = ref lights[l];
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
		private static void RenderFrames(RawMovie movie, Obj[] objs, PointLight[] initialLights, Color ambient, ref int nextFrame)
		{
			var camera = new Camera(new Vec3(0, 0, 0), new Vec3(0, 0, -1), new Vec3(0, 1, 0));
			var lights = (PointLight[])initialLights.Clone();
			int width = movie.width;
			int height = movie.height;
			Num widthRange = width - 1;
			Num heightRange = height - 1;

			// each worker claims a complete frame when it is ready for more work
			int f;
			while ((f = Interlocked.Increment(ref nextFrame)) < movie.frames.Length)
			{
				Console.WriteLine($"Frame:{f} rendering");
				UpdateMovieObjects(camera, lights, f);

				Vec3 rayOrigin = camera.position;
				Mat3 camMatrix = camera.matrix;
				var colors = movie.frames[f].colors;
				// each frame has its own random sequence regardless of which worker renders it
				Num ox = f * .5f;
				Num oy = f * .75f;
				for (int y = 0; y < height; ++y)
				{
					var row = colors.AsSpan(y * width, width);
					Num yNorm = y / heightRange;
					for (int x = 0; x < row.Length; ++x)
					{
						Num xNorm = x / widthRange;
						Vec3 rayDirection = new Vec3(xNorm - .5f, yNorm - .5f, -1).Normalize();
						rayDirection = camMatrix.Transform(rayDirection);

						// generate random vector
						Num randomVal = RandomUtil.NextValue(xNorm, yNorm, ref ox, ref oy) - .5f;
						var randomVec = new Vec3(randomVal, randomVal, randomVal);

						// trace
						var light = new Color();
						Trace(rayOrigin, rayDirection, ref light, maxBounceCount, randomVec, objs, lights, ambient);

						// write result
						row[x] = light;
					}
				}
			}
		}

		[MethodImpl(MethodImplOptions.AggressiveOptimization)]
		static void Trace(Vec3 rayOrigin, Vec3 rayDirection, ref Color light, int maxBounce, Vec3 randomVec, Obj[] objs, PointLight[] lights, Color ambient)
		{
			// find closest object
			int closestObjectIdx = -1;
			Num closestDis = Num.MaxValue;
			Vec3 closestPoint = default;
			Vec3 closestNormal = default;
			Num a = rayDirection.Dot();
			for (int o = 0; o < objs.Length; ++o)
			{
				if (objs[o].Hit(rayOrigin, rayDirection, a, out var infoPoint, out var infoNormal))
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
				if (Ray.HitSphere(rayOrigin, rayDirection, pointLight.position, pointLight.radius * pointLight.radius, a, out var infoPoint, out _) &&
					(rayOrigin - infoPoint).Length() < closestDis)
				{
					light = pointLight.color;
					return;
				}
			}

			// process lighting
			if (closestObjectIdx >= 0)
			{
				if (maxBounce <= 0) return;
				ref readonly var closestObject = ref objs[closestObjectIdx];
				ref readonly var material = ref closestObject.material;

				// diffuse surface normal
				Vec3 reflectedNormal = closestNormal + ((randomVec * material.roughness) / (closestDis + 1));
				Vec3 reflectedDirection = rayDirection.Reflect(reflectedNormal.Normalize());
				Vec3 reflectedOrigin = closestPoint - (reflectedDirection * .0001f);// move a little off surface to correct for floating point errors

				// gather surface light
				GatherLight(in closestObject, closestPoint, reflectedNormal, ref light, randomVec, objs, lights, ambient);

				// trace from this reflected path
				var reflectedLight = new Color();
				Trace(reflectedOrigin, reflectedDirection, ref reflectedLight, maxBounce - 1, randomVec, objs, lights, ambient);

				// material (reflection)
				Num fresnel = 1 - Vec3.FresnelSchlickZero(rayDirection, closestNormal);
				light += Color.Lerp(reflectedLight, reflectedLight * material.color, material.metalic * fresnel) * material.reflectiveness;
			}

			// volume fog tracing taking into account shadows
			#if !DISABLE_FOG
			int fogSteps = closestObjectIdx >= 0 ? (int)MATH.Min(closestDis / volumeFogStep, maxVolumeFogSteps) : maxVolumeFogSteps;
			GatherFog(rayOrigin, rayDirection, ref light, randomVec, objs, lights, fogSteps);
			#endif
		}

		#if !DISABLE_FOG
		[MethodImpl(MethodImplOptions.AggressiveOptimization)]
		static void GatherFog(Vec3 rayOrigin, Vec3 rayDirection, ref Color light, Vec3 randomVec, Obj[] objs, PointLight[] lights, int steps)
		{
			var fogStep = rayDirection * volumeFogStep;
			for (int l = 0; l < lights.Length; ++l)
			{
				var fogPos = rayOrigin;
				Num fogBrightness = 0;
				ref readonly var pointLight = ref lights[l];
				var lightPosition = pointLight.position + (randomVec * pointLight.radius);
				for (int i = 0; i < steps; ++i)
				{
					fogPos += fogStep;
					Vec3 direction = lightPosition - fogPos;
					Num distanceSquared = direction.Dot();
					if (!IsInShadow(fogPos, direction, distanceSquared, objs))
					{
						fogBrightness += 1 / MATH.Pow4(MATH.Sqrt(distanceSquared) + 1);
					}
				}
				light += pointLight.color * fogBrightness * volumeFogStep * .1f;
			}
		}
		#endif

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
				Vec3 direction = (pointLight.position + (randomVec * pointLight.radius)) - infoPoint;
				Num distanceSquared = direction.Dot();
				if (!IsInShadow(infoPoint, direction, distanceSquared, objs))
				{
					Num lightDis = MATH.Sqrt(distanceSquared);
					Num d = MATH.Max(infoNormal.Dot(direction / lightDis), 0);
					d /= MATH.Pow4(lightDis + 1);
					light += objColor * pointLight.color * d;
				}
			}
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		static bool IsInShadow(Vec3 point, Vec3 direction, Num distanceSquared, Obj[] objs)
		{
			// A ray parameter of 1 is the light position.
			for (int o = 0; o < objs.Length; ++o)
			{
				if (objs[o].BlocksLight(point, direction, distanceSquared)) return true;
			}
			return false;
		}
	}
}