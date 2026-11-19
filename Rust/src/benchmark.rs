//#![forbid(unsafe_code)]
#![allow(dead_code, non_upper_case_globals)]

use std::ops::{Add, AddAssign, Div, Mul, Neg, Sub};
use std::sync::Mutex;
use std::thread;

// Change to f64 for NUM64
pub type Num = f32;
pub const DISABLE_FOG: bool = false;

pub struct RandomUtil
{
    offset_x: Num,
    offset_y: Num,
}

impl RandomUtil
{
    #[inline(always)]
    pub fn for_frame(frame: i32) -> Self
    {
        Self {
            offset_x: frame as Num * 0.5,
            offset_y: frame as Num * 0.75,
        }
    }

    #[inline(always)]
    pub fn next_value(&mut self, mut u: Num, mut v: Num) -> Num
    {
        const randoValueX: Num = 12.9898;
        const randoValueY: Num = 78.233;
        const randoValueZ: Num = 43758.543123;

        u += self.offset_x;
        v += self.offset_y;
        self.offset_x += 0.5;
        self.offset_y += 0.75;

        let mut result = (u * randoValueX) + (v * randoValueY);
        result = result.sin() * randoValueZ;
        result - result.floor()
    }
}

#[derive(Copy, Clone, Default)]
pub struct Color
{
    pub r: Num,
    pub g: Num,
    pub b: Num,
}

impl Color
{
    #[inline(always)]
    pub const fn new(r: Num, g: Num, b: Num) -> Self
    {
        Self { r, g, b }
    }

    #[inline(always)]
    pub fn lerp(p1: Color, p2: Color, t: Num) -> Color
    {
        p1 + ((p2 - p1) * t)
    }

    #[inline(always)]
    pub fn lerp_clamp(p1: Color, p2: Color, t: Num) -> Color
    {
        p1 + ((p2 - p1) * t.clamp(0.0, 1.0))
    }
}

impl Add for Color
 {
    type Output = Color;
    #[inline(always)]
    fn add(self, p2: Color) -> Color
    {
        Color::new(
            self.r + p2.r,
            self.g + p2.g,
            self.b + p2.b
        )
    }
}

impl AddAssign for Color
{
    #[inline(always)]
    fn add_assign(&mut self, p1: Color)
    {
        self.r += p1.r;
        self.g += p1.g;
        self.b += p1.b;
    }
}

impl Sub for Color
{
    type Output = Color;
    
    #[inline(always)]
    fn sub(self, p2: Color) -> Color
    {
        Color::new(
            self.r - p2.r,
            self.g - p2.g,
            self.b - p2.b
        )
    }
}

impl Mul for Color
{
    type Output = Color;
    
    #[inline(always)]
    fn mul(self, p2: Color) -> Color
    {
        Color::new(
            self.r * p2.r,
            self.g * p2.g,
            self.b * p2.b
        )
    }
}

impl Mul<Num> for Color
{
    type Output = Color;
    
    #[inline(always)]
    fn mul(self, p2: Num) -> Color {
        Color::new(
            self.r * p2,
            self.g * p2,
            self.b * p2
        )
    }
}

impl Div<Num> for Color
{
    type Output = Color;
    
    #[inline(always)]
    fn div(self, p2: Num) -> Color
    {
        Color::new(
            self.r / p2,
            self.g / p2,
            self.b / p2
        )
    }
}

#[derive(Copy, Clone, Default)]
pub struct Vec3
{
    pub x: Num,
    pub y: Num,
    pub z: Num,
}

impl Vec3
{
    #[inline(always)]
    pub const fn new(x: Num, y: Num, z: Num) -> Self {
        Self { x, y, z }
    }

    #[inline(always)]
    pub fn dot_self(self) -> Num
    {
        (self.x * self.x) +
        (self.y * self.y) +
        (self.z * self.z)
    }

    #[inline(always)]
    pub fn dot(self, vec: Vec3) -> Num
    {
        (self.x * vec.x) +
        (self.y * vec.y) +
        (self.z * vec.z)
    }

    #[inline(always)]
    pub fn length(self) -> Num
    {
        ((self.x * self.x) + (self.y * self.y) + (self.z * self.z)).sqrt()
    }

    #[inline(always)]
    pub fn normalize(self) -> Vec3
    {
        self / ((self.x * self.x) + (self.y * self.y) + (self.z * self.z)).sqrt()
    }

    #[inline(always)]
    pub fn reflect(self, plane_normal: Vec3) -> Vec3
    {
        self - (plane_normal * self.dot(plane_normal) * 2.0)
    }

    #[inline(always)]
    pub fn cross(self, vector: Vec3) -> Vec3
    {
        Vec3::new(
            (self.y * vector.z) - (self.z * vector.y),
            (self.z * vector.x) - (self.x * vector.z),
            (self.x * vector.y) - (self.y * vector.x),
        )
    }

    #[inline(always)]
    pub fn fresnel_schlick_zero(ray_direction: Vec3, normal: Vec3) -> Num
    {
        (1.0 - (-ray_direction).dot(normal).max(0.0)).powi(5)
    }
}

impl Add for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn add(self, p2: Vec3) -> Vec3
    {
        Vec3::new(
            self.x + p2.x,
            self.y + p2.y,
            self.z + p2.z
        )
    }
}

impl AddAssign for Vec3
{
    #[inline(always)]
    fn add_assign(&mut self, p1: Vec3)
    {
        self.x += p1.x;
        self.y += p1.y;
        self.z += p1.z;
    }
}

impl Sub for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn sub(self, p2: Vec3) -> Vec3
    {
        Vec3::new(
            self.x - p2.x,
            self.y - p2.y,
            self.z - p2.z)
    }
}

impl Sub<Num> for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn sub(self, p2: Num) -> Vec3
    {
        Vec3::new(
            self.x - p2,
            self.y - p2,
            self.z - p2
        )
    }
}

impl Neg for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn neg(self) -> Vec3
    {
        Vec3::new(
            -self.x,
            -self.y,
            -self.z
        )
    }
}

impl Mul for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn mul(self, p2: Vec3) -> Vec3
    {
        Vec3::new(
            self.x * p2.x,
            self.y * p2.y,
            self.z * p2.z
        )
    }
}

impl Mul<Num> for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn mul(self, p2: Num) -> Vec3
    {
        Vec3::new(
            self.x * p2,
            self.y * p2,
            self.z * p2
        )
    }
}

impl Mul<Vec3> for Num
{
    type Output = Vec3;
    
    #[inline(always)]
    fn mul(self, p2: Vec3) -> Vec3
    {
        Vec3::new(
            self * p2.x,
            self * p2.y,
            self * p2.z
        )
    }
}

impl Div for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn div(self, p2: Vec3) -> Vec3
    {
        Vec3::new(
            self.x / p2.x,
            self.y / p2.y,
            self.z / p2.z
        )
    }
}

impl Div<Num> for Vec3
{
    type Output = Vec3;
    
    #[inline(always)]
    fn div(self, p2: Num) -> Vec3
    {
        Vec3::new(
            self.x / p2,
            self.y / p2,
            self.z / p2
        )
    }
}

#[derive(Copy, Clone, Default)]
pub struct Mat3
{
    pub x: Vec3,
    pub y: Vec3,
    pub z: Vec3,
}

impl Mat3
{
    #[inline(always)]
    pub fn transform(&self, v: Vec3) -> Vec3
    {
        (self.x * v.x) +
        (self.y * v.y) +
        (self.z * v.z)
    }
}

#[derive(Copy, Clone, Default)]
pub struct HitInfo
{
    pub point: Vec3,
    pub normal: Vec3,
}

#[derive(Copy, Clone, Default)]
pub struct Ray
{
    pub origin: Vec3,
    pub direction: Vec3,
}

impl Ray
{
    #[inline(always)]
    pub const fn new(origin: Vec3, direction: Vec3) -> Self {
        Self { origin, direction }
    }

    #[inline(always)]
    fn hit_sphere_numerator(&self, sphere_center: Vec3, radius_squared: Num, a: Num) -> Option<Num>
    {
        let d = self.origin - sphere_center;
        let b = d.dot(self.direction);
        if b >= 0.0 { return None; }

        let c = d.dot_self() - radius_squared;
        let disc = b * b - a * c;
        if disc < 0.0 { return None; }

        Some(-b - disc.sqrt())
    }

    #[inline(always)]
    pub fn hit_sphere(&self, sphere_center: Vec3, radius_squared: Num, a: Num, info: &mut HitInfo) -> bool
    {
        let Some(numerator) = self.hit_sphere_numerator(sphere_center, radius_squared, a) else
        {
            *info = HitInfo::default();
            return false;
        };

        let t = numerator * (1.0 / a);
        info.point = self.origin + t * self.direction;
        info.normal = (info.point - sphere_center).normalize();

        true
    }
}

#[derive(Copy, Clone, Default)]
pub struct Material
{
    pub color: Color,
    pub roughness: Num,
    pub reflectiveness: Num,
    pub metalic: Num,
}

impl Material
{
    #[inline(always)]
    pub const fn new(color: Color, roughness: Num, reflectiveness: Num, metalic: Num) -> Self
    {
        Self {
            color,
            roughness,
            reflectiveness,
            metalic,
        }
    }
}

#[derive(Copy, Clone)]
pub enum ObjType
{
    Plane(Plane),
    Sphere(Sphere),
}

#[derive(Copy, Clone)]
pub struct Obj
{
    pub obj_type: ObjType,
    pub material: Material,
}

impl Obj
{
    #[inline(always)]
    pub fn new_plane(center: Vec3, normal: Vec3, material: Material) -> Self
    {
        Self {
            obj_type: ObjType::Plane(Plane::new(center, normal)),
            material,
        }
    }

    #[inline(always)]
    pub fn new_sphere(center: Vec3, radius: Num, material: Material) -> Self
    {
        Self {
            obj_type: ObjType::Sphere(Sphere::new(center, radius)),
            material,
        }
    }

    #[inline(always)]
    pub fn hit(&self, ray: &Ray, a: Num, info: &mut HitInfo) -> bool
    {
        match &self.obj_type
        {
            ObjType::Sphere(sphere) => sphere.hit(ray, a, info),
            ObjType::Plane(plane) => plane.hit(ray, info),
        }
    }

    #[inline(always)]
    fn blocks_light(&self, ray: &Ray, a: Num) -> bool
    {
        match &self.obj_type
        {
            ObjType::Sphere(sphere) =>
            {
                // The hit parameter is numerator / a.
                matches!(ray.hit_sphere_numerator(sphere.center, sphere.radius_squared, a),
                    Some(numerator) if numerator.abs() <= a)
            }
            ObjType::Plane(plane) => plane.blocks_light(ray, a),
        }
    }
}

#[derive(Copy, Clone)]
pub struct Sphere
{
    pub center: Vec3,
    radius_squared: Num,
}

impl Sphere
{
    #[inline(always)]
    pub fn new(center: Vec3, radius: Num) -> Self
    {
        Self { center, radius_squared: radius * radius }
    }

    #[inline(always)]
    pub fn hit(&self, ray: &Ray, a: Num, info: &mut HitInfo) -> bool
    {
        ray.hit_sphere(self.center, self.radius_squared, a, info)
    }
}

#[derive(Copy, Clone)]
pub struct Plane
{
    pub center: Vec3,
    pub normal: Vec3,
}

impl Plane
{
    const PARALLEL_THRESHOLD: Num = 0.0001;

    #[inline(always)]
    pub fn new(center: Vec3, normal: Vec3) -> Self
    {
        Self {
            center,
            normal: normal.normalize(),
        }
    }

    #[inline(always)]
    pub fn hit(&self, ray: &Ray, info: &mut HitInfo) -> bool {
        let denom = self.normal.dot(ray.direction);
        if denom < -Self::PARALLEL_THRESHOLD
        {
            let t = (self.center - ray.origin).dot(self.normal) / denom;
            if t >= 0.0
            {
                info.point = ray.origin + (ray.direction * t);
                info.normal = self.normal;
                return true;
            }
        }

        *info = HitInfo::default();
        false
    }

    #[inline(always)]
    fn blocks_light(&self, ray: &Ray, a: Num) -> bool
    {
        let denom = self.normal.dot(ray.direction);
        if denom >= 0.0 || denom * denom <= a * (Self::PARALLEL_THRESHOLD * Self::PARALLEL_THRESHOLD)
        {
            return false;
        }

        let offset = (self.center - ray.origin).dot(self.normal);
        offset <= 0.0 && offset >= denom
    }
}

pub struct RawImage
{
    pub width: i32,
    pub height: i32,
    pub colors: Vec<Color>,
}

impl RawImage
{
    #[inline(always)]
    pub fn new(width: i32, height: i32) -> Self
    {
        Self {
            width,
            height,
            colors: vec![Color::default(); (width * height) as usize],
        }
    }
}

pub struct RawMovie
{
    pub width: i32,
    pub height: i32,
    pub frame_count: i32,
    pub frames: Vec<RawImage>,
}

impl RawMovie
{
    pub fn new(width: i32, height: i32, frame_count: i32) -> Self
    {
        let mut frames = Vec::with_capacity(frame_count as usize);
        for _ in 0..frame_count
        {
            frames.push(RawImage::new(width, height));
        }
        
        Self {
            width,
            height,
            frame_count,
            frames,
        }
    }
}

#[derive(Clone)]
pub struct Camera
{
    pub position: Vec3,
    pub forward: Vec3,
    pub up: Vec3,
    pub matrix: Mat3,
}

impl Camera
{
    pub fn new(position: Vec3, forward: Vec3, up: Vec3) -> Self
    {
        Self {
            position,
            forward: forward.normalize(),
            up: up.normalize(),
            matrix: Mat3::default(),
        }
    }

    pub fn update(&mut self)
    {
        let f = -self.forward;
        let r = self.up.cross(f);
        let u = f.cross(r);
        
        self.matrix.x = r.normalize();
        self.matrix.y = u.normalize();
        self.matrix.z = f.normalize();
    }
}

#[derive(Clone)]
pub struct PointLight
{
    pub position: Vec3,
    pub radius: Num,
    pub color: Color,
    pub rot_pos: Vec3,
    pub rot_speed: Num,
}

impl PointLight
{
    pub fn new(position: Vec3, radius: Num, color: Color, rot_speed: Num) -> Self
    {
        Self {
            position,
            radius,
            color,
            rot_pos: position,
            rot_speed,
        }
    }
}

#[derive(Clone)]
pub struct Scene
{
    pub camera: Camera,
    pub objects: Vec<Obj>,
    pub ambient_light: Color,
    pub point_lights: Vec<PointLight>,
    pub fog_color: Color,
}

const maxBounceCount: i32 = 10; // how many ray-bounces are allowed
const roughnessMul: Num = 1.0; // force tween material value

const maxVolumeFogSteps: i32 = 250 / 4; // how far can we test the shadow volume for
const volumeFogStep: Num = 0.01 * 4.0; // how little the shadow volume testing step is
const fogDensity: Num = 0.4; // how dense the fog is
const fogFalloff: Num = 1.0;

impl Scene
{
    pub fn update_movie_objects(&mut self, frame: i32)
    {
        let rot = frame as Num * 0.1;
        for point_light in &mut self.point_lights
        {
            let r = rot * point_light.rot_speed;
            point_light.position = point_light.rot_pos + (Vec3::new(r.cos(), r.sin() * 4.0, r.sin() * 3.0) * 0.1);
            point_light.color = Color::new(
                r.cos().abs() * 10.0,
                r.sin().abs() * 10.0,
                ((r * 0.33).cos() * 10.0).abs(),
            );
        }

        let c = -rot * 0.25;
        let center = Vec3::new(0.0, 0.0, -2.0);
        self.camera.position = center + Vec3::new(c.sin(), c.sin() * 0.1, -c.cos()) * 2.0;
        self.camera.forward = (center - self.camera.position).normalize();
        self.camera.update();
    }

    fn render_frame(&mut self, frame: i32, image: &mut RawImage)
    {
        println!("Frame:{} rendering", frame);
        self.update_movie_objects(frame);

        if image.width <= 0 || image.height <= 0 { return; }

        let width_range = (image.width - 1) as Num;
        let height_range = (image.height - 1) as Num;
        let mut ray = Ray::new(self.camera.position, Vec3::default());
        let mut random = RandomUtil::for_frame(frame);

        for (y, row) in image.colors.chunks_exact_mut(image.width as usize).enumerate()
        {
            let v = y as Num / height_range;
            for (x, pixel) in row.iter_mut().enumerate()
            {
                let u = x as Num / width_range;
                ray.direction = Vec3::new(u - 0.5, v - 0.5, -1.0).normalize();
                ray.direction = self.camera.matrix.transform(ray.direction);

                let random_val = random.next_value(u, v) - 0.5;
                let random_vec = Vec3::new(random_val, random_val, random_val);

                let mut light = Color::default();
                self.trace(&ray, &mut light, maxBounceCount, &random_vec);
                *pixel = light;
            }
        }
    }

    fn trace(&self, ray: &Ray, light: &mut Color, max_bounce: i32, random_vec: &Vec3)
    {
        // find closest object
        let mut closest_object = None;
        let mut closest_dis = Num::MAX;
        let mut closest_info = HitInfo::default();
        let a = ray.direction.dot_self();

        for obj in &self.objects
        {
            let mut info = HitInfo::default();
            if obj.hit(ray, a, &mut info)
            {
                let dis = (ray.origin - info.point).length();
                if dis < closest_dis
                {
                    closest_dis = dis;
                    closest_object = Some(obj);
                    closest_info = info;
                }
            }
        }

        // hit point light orb
        for point_light in &self.point_lights
        {
            let radius_squared = point_light.radius * point_light.radius;
            if let Some(numerator) = ray.hit_sphere_numerator(point_light.position, radius_squared, a)
            {
                let t = numerator * (1.0 / a);
                let point = ray.origin + t * ray.direction;
                if (ray.origin - point).length() < closest_dis
                {
                    *light = point_light.color;
                    return;
                }
            }
        }

        // process lighting
        if let Some(closest_obj) = closest_object
        {
            if max_bounce <= 0 { return; }

            let mut reflected_closest_info = closest_info;
            let mut reflected_ray = *ray;
            let material = &closest_obj.material;

            // diffuse surface normal
            reflected_closest_info.normal = closest_info.normal
                + ((*random_vec * material.roughness) / (closest_dis + 1.0));
            reflected_ray.direction = ray
                .direction
                .reflect(reflected_closest_info.normal.normalize());
            reflected_ray.origin = closest_info.point - (reflected_ray.direction * 0.0001); // move a little off surface to correct for floating point errors

            self.gather_light(closest_obj, &reflected_closest_info, light, random_vec);

            // trace from this reflected path
            let mut reflected_light = Color::default();
            self.trace(&reflected_ray, &mut reflected_light, max_bounce - 1, random_vec);

            // material (reflection)
            let fresnel = 1.0 - Vec3::fresnel_schlick_zero(ray.direction, closest_info.normal);
            *light += Color::lerp(
                reflected_light,
                reflected_light * material.color,
                material.metalic * fresnel,
            ) * material.reflectiveness;
        }

        // volume fog tracing taking into account shadows
        if !DISABLE_FOG
        {
            let steps = if closest_object.is_some()
            {
                (closest_dis / volumeFogStep).min(maxVolumeFogSteps as Num) as i32
            }
            else
            {
                maxVolumeFogSteps
            };
            self.gather_fog(ray, light, random_vec, steps);
        }
    }

    fn gather_fog(&self, ray: &Ray, light: &mut Color, random_vec: &Vec3, steps: i32)
    {
        let mut fog_pos = ray.origin;
        let fog_step = ray.direction * volumeFogStep;
        for point_light in &self.point_lights
        {
            let mut fog_brightness: Num = 0.0;
            let light_position = point_light.position + (*random_vec * point_light.radius);
            for _ in 0..steps
            {
                fog_pos += fog_step;
                let light_ray = Ray::new(fog_pos, light_position - fog_pos);
                let distance_squared = light_ray.direction.dot_self();
                if !self.is_in_shadow(&light_ray, distance_squared)
                {
                    fog_brightness += 1.0 / (distance_squared.sqrt() + 1.0).powi(4);
                }
            }
            *light += point_light.color * fog_brightness * volumeFogStep * 0.1;
        }
    }

    #[inline(always)]
    fn gather_light(&self, obj: &Obj, info: &HitInfo, light: &mut Color, random_vec: &Vec3)
    {
        let obj_color = obj.material.color;

        // ambient
        *light += obj_color * self.ambient_light;

        // point
        for point_light in &self.point_lights
        {
            let light_position = point_light.position + (*random_vec * point_light.radius);
            let light_ray = Ray::new(info.point, light_position - info.point);
            let distance_squared = light_ray.direction.dot_self();
            if !self.is_in_shadow(&light_ray, distance_squared)
            {
                let light_dis = distance_squared.sqrt();
                let mut d = info.normal.dot(light_ray.direction / light_dis).max(0.0);
                d /= (light_dis + 1.0).powi(4);
                *light += obj_color * point_light.color * d;
            }
        }
    }

    #[inline(always)]
    fn is_in_shadow(&self, ray: &Ray, distance_squared: Num) -> bool
    {
        // A ray parameter of 1 is the light position.
        self.objects.iter().any(|obj| obj.blocks_light(ray, distance_squared))
    }
}

pub struct Benchmark
{
    pub movie: RawMovie,
    pub scene: Scene,
    pub thread_count: usize,
}

impl Benchmark
{
    pub fn init(width: i32, height: i32, mut frames: i32) -> Self
    {
        if frames < 1 { frames = 1; }
        
        let movie = RawMovie::new(width, height, frames);
        let camera = Camera::new(
            Vec3::new(0.0, 0.0, 0.0),
            Vec3::new(0.0, 0.0, -1.0),
            Vec3::new(0.0, 1.0, 0.0),
        );

        let objects = vec![
            Obj::new_plane(
                Vec3::new(0.0, -0.4, 0.0),
                Vec3::new(0.0, 1.0, 0.0),
                Material::new(Color::new(0.3, 0.3, 0.3), 0.03 * roughnessMul, 1.0, 0.0),
            ),
            Obj::new_sphere(
                Vec3::new(0.0, 0.0, -2.0),
                0.33,
                Material::new(Color::new(1.0, 0.0, 0.0), 0.0 * roughnessMul, 0.25, 0.0),
            ),
            Obj::new_sphere(
                Vec3::new(-0.25, 0.5, -1.5),
                0.25,
                Material::new(Color::new(0.0, 1.0, 0.0), 0.0 * roughnessMul, 1.0, 1.0),
            ),
            Obj::new_sphere(
                Vec3::new(1.0, -0.75, -2.0),
                1.0,
                Material::new(Color::new(0.0, 0.0, 1.0), 0.0 * roughnessMul, 1.0, 1.0),
            ),
            Obj::new_sphere(
                Vec3::new(0.2, 0.25, -1.5),
                0.1,
                Material::new(Color::new(1.0, 1.0, 0.0), 0.0 * roughnessMul, 1.0, 0.5),
            ),
        ];

        let ambient_light = Color::new(0.05, 0.05, 0.05);
        let point_lights = vec![PointLight::new(
            Vec3::new(0.1, 0.75, -2.0),
            0.01,
            Color::new(1.0, 1.0, 1.0) * 10.0,
            1.0,
        )];

        let scene = Scene {
            camera,
            objects,
            ambient_light,
            point_lights,
            fog_color: Color::new(0.25, 0.25, 0.25),
        };

        let thread_count = thread::available_parallelism()
            .map(|n| n.get())
            .unwrap_or(1)
            .min(frames as usize);
        println!("Thread Count: {}", thread_count);

        Self { movie, scene, thread_count }
    }

    pub fn render(&mut self, single_threaded: bool) -> &RawMovie
    {
        let worker_count = if single_threaded { 1 } else { self.thread_count };
        let initial_scene = &self.scene;
        let frames = Mutex::new(self.movie.frames.iter_mut().enumerate());

        let render_frames = ||
        {
            let mut scene = initial_scene.clone();
            loop
            {
                // Each worker takes ownership of a frame and fills its pixels in place.
                let next = frames.lock().unwrap().next();
                let Some((index, image)) = next else { break; };
                scene.render_frame(index as i32, image);
            }
        };

        thread::scope(|scope| {
            for _ in 1..worker_count
            {
                if thread::Builder::new().spawn_scoped(scope, &render_frames).is_err() { break; }
            }
            render_frames();
        });

        &self.movie
    }
}
