//#![forbid(unsafe_code)]
#![allow(dead_code, non_upper_case_globals)]

use std::cell::Cell;
use std::ops::{Add, AddAssign, Div, Mul, Neg, Sub};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex, RwLock};
use std::thread::{self, JoinHandle};
use std::time::Duration;

// Change to f64 for NUM64
pub type Num = f32;
pub const DISABLE_FOG: bool = false;

pub struct RandomUtil;

thread_local!
{
    static OFFSET: Cell<(Num, Num)> = const { Cell::new((0.0, 0.0)) };
}

impl RandomUtil
{
    #[inline(always)]
    pub fn next_value(mut u: Num, mut v: Num) -> Num
    {
        const randoValueX: Num = 12.9898;
        const randoValueY: Num = 78.233;
        const randoValueZ: Num = 43758.543123;

        OFFSET.with(|offset| {
            let (ox, oy) = offset.get();
            u += ox;
            v += oy;
            offset.set((ox + 0.5, oy + 0.75));
        });

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
    pub fn hit_sphere(&self, sphere_center: Vec3, sphere_radius: Num, info: &mut HitInfo) -> bool
    {
        // if ray pointing wrong direction ignore
        if self.direction.dot(self.origin - sphere_center) >= 0.0
        {
            *info = HitInfo::default();
            return false;
        }

        // compute
        let d = self.origin - sphere_center;
        let a = self.direction.dot(self.direction);
        let b = d.dot(self.direction);
        let c = d.dot_self() - sphere_radius * sphere_radius;

        // if ray not pointing at object ignore
        let disc = b * b - a * c;
        if disc < 0.0
        {
            *info = HitInfo::default();
            return false;
        }

        // compute
        let sqrt_disc = disc.sqrt();
        let inv_a = 1.0 / a;
        let t1 = (-b - sqrt_disc) * inv_a;

        info.point = self.origin + t1 * self.direction;
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
    pub fn hit(&self, ray: &Ray, info: &mut HitInfo) -> bool
    {
        match &self.obj_type
        {
            ObjType::Sphere(sphere) => sphere.hit(ray, info),
            ObjType::Plane(plane) => plane.hit(ray, info),
        }
    }
}

#[derive(Copy, Clone)]
pub struct Sphere
{
    pub center: Vec3,
    pub radius: Num,
}

impl Sphere
{
    #[inline(always)]
    pub fn new(center: Vec3, radius: Num) -> Self
    {
        Self { center, radius }
    }

    #[inline(always)]
    pub fn hit(&self, ray: &Ray, info: &mut HitInfo) -> bool
    {
        ray.hit_sphere(self.center, self.radius, info)
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
        // if ray pointing wrong direction ignore
        if ray.direction.dot(self.normal) >= 0.0
        {
            *info = HitInfo::default();
            return false;
        }

        let denom = self.normal.dot(ray.direction);
        if denom.abs() > 0.0001
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

pub struct RenderThreadData
{
    pub index: i32,
    pub random_index: i32,
    pub waiting: AtomicBool,
    pub done: AtomicBool,
    pub exit: AtomicBool,
    pub x: i32,
    pub y: i32,
    pub x_end: i32,
    pub y_end: i32,
    pub buffer: Mutex<Vec<Color>>,
}

pub struct Scene
{
    pub width: i32,
    pub height: i32,
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

    pub fn trace(&self, ray: &Ray, light: &mut Color, max_bounce: &mut i32, random_vec: &Vec3) -> bool
    {
        // find closest object
        let mut closest_object: Option<&Obj> = None;
        let mut closest_dis = Num::MAX;
        let mut closest_info = HitInfo::default();
        
        for obj in &self.objects
        {
            let mut info = HitInfo::default();
            if obj.hit(ray, &mut info)
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
            let mut closest_light: Option<&PointLight> = None;
            let mut info = HitInfo::default();
            if ray.hit_sphere(point_light.position, point_light.radius, &mut info)
            {
                let dis = (ray.origin - info.point).length();
                if dis < closest_dis
                {
                    closest_dis = dis;
                    closest_light = Some(point_light);
                    closest_info = info;
                }
            }

            if let Some(cl) = closest_light
            {
                *light = cl.color;
                return true;
            }
        }

        // process lighting
        if let Some(closest_obj) = closest_object
        {
            if *max_bounce <= 0 { return false; }
            
            *max_bounce -= 1;
            let max_bounce_reset = *max_bounce;

            // buffer diffuse pass state objects
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

            // gather surface light
            self.gather_light(closest_obj, &reflected_closest_info, light, random_vec);

            // trace from this reflected path
            *max_bounce = max_bounce_reset; // reset max bounce here or we lose light average consistancy
            let mut reflected_light = Color::default();
            self.trace(
                &reflected_ray,
                &mut reflected_light,
                max_bounce,
                random_vec,
            );

            // material (reflection)
            let fresnel = 1.0 - Vec3::fresnel_schlick_zero(ray.direction, closest_info.normal);
            *light += Color::lerp(
                reflected_light,
                reflected_light * material.color,
                material.metalic * fresnel,
            ) * material.reflectiveness;

            // volume fog tracing taking into account shadows
            if !DISABLE_FOG
            {
                let mut fog_pos = ray.origin;
                let fog_step = ray.direction * volumeFogStep;
                let max_volume_fog_steps_clamped =
                    (closest_dis / volumeFogStep).min(maxVolumeFogSteps as Num) as i32;
                
                for point_light in &self.point_lights
                {
                    let mut fog_brightness: Num = 0.0;
                    
                    for _ in 0..max_volume_fog_steps_clamped
                    {
                        fog_pos += fog_step;
                        let mut light_dis: Num = 0.0;
                        let mut light_ray = Ray::default();
                        if !self.is_in_shadow(
                            point_light,
                            fog_pos,
                            *random_vec,
                            &mut light_dis,
                            &mut light_ray,
                        ) {
                            fog_brightness += 1.0 / (light_dis + 1.0).powi(4);
                        }
                    }
                    *light += point_light.color * fog_brightness * volumeFogStep * 0.1;
                }
            }

            return true;
        }
        else
        {
            // volume fog tracing taking into account shadows (optimized for when nothing is hit)
            if !DISABLE_FOG
            {
                let mut fog_pos = ray.origin;
                let fog_step = ray.direction * volumeFogStep;
                
                for point_light in &self.point_lights
                {
                    let mut fog_brightness: Num = 0.0;
                    for _ in 0..maxVolumeFogSteps
                    {
                        fog_pos += fog_step;
                        let mut light_dis: Num = 0.0;
                        let mut light_ray = Ray::default();
                        if !self.is_in_shadow(
                            point_light,
                            fog_pos,
                            *random_vec,
                            &mut light_dis,
                            &mut light_ray,
                        ) {
                            fog_brightness += 1.0 / (light_dis + 1.0).powi(4);
                        }
                    }
                    *light += point_light.color * fog_brightness * volumeFogStep * 0.1;
                }
            }
        }

        // hit nothing
        false
    }

    #[inline(always)]
    pub fn gather_light(&self, obj: &Obj, info: &HitInfo, light: &mut Color, random_vec: &Vec3)
    {
        let obj_color = obj.material.color;

        // ambient
        *light += obj_color * self.ambient_light;

        // point
        for point_light in &self.point_lights
        {
            // diffuse
            let mut light_dis: Num = 0.0;
            let mut light_ray = Ray::default();
            if !self.is_in_shadow(
                point_light,
                info.point,
                *random_vec,
                &mut light_dis,
                &mut light_ray,
            ) {
                let mut d = info.normal.dot(light_ray.direction).max(0.0);
                d /= (light_dis + 1.0).powi(4);
                *light += obj_color * point_light.color * d;
            }
        }
    }

    #[inline(always)]
    pub fn is_in_shadow(
        &self,
        point_light: &PointLight,
        point: Vec3,
        random_vec: Vec3,
        light_dis: &mut Num,
        light_ray: &mut Ray,
    ) -> bool {
        let soft_vec = (point_light.position + (random_vec * point_light.radius)) - point;
        *light_dis = soft_vec.length();
        *light_ray = Ray::new(point, soft_vec.normalize());
        
        for obj in &self.objects
        {
            let mut hit_info = HitInfo::default();
            if obj.hit(light_ray, &mut hit_info) && (hit_info.point - point).length() <= *light_dis
            {
                return true;
            }
        }
        false
    }
}

pub struct Benchmark
{
    pub width: i32,
    pub height: i32,
    pub frames: i32,
    pub movie: RawMovie,
    pub scene: Arc<RwLock<Scene>>,
    pub thread_count: i32,
    pub thread_datas: Vec<Arc<RenderThreadData>>,
    pub threads: Vec<JoinHandle<()>>,
    pub threads_alive: bool,
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

        let scene = Arc::new(RwLock::new(Scene {
            width,
            height,
            camera,
            objects,
            ambient_light,
            point_lights,
            fog_color: Color::new(0.25, 0.25, 0.25),
        }));

        // start threads
        let thread_count = thread::available_parallelism()
            .map(|n| n.get() as i32)
            .unwrap_or(1);
        
        println!("Thread Count: {}", thread_count);
        
        let mut thread_datas = Vec::with_capacity(thread_count as usize);
        let mut threads = Vec::with_capacity(thread_count as usize);

        let section_height = height / thread_count;
        
        for i in 0..thread_count
        {
            let y_start = i * section_height;
            let mut y_end = y_start + section_height;
            if i == thread_count - 1 { y_end = height; }
            
            let strip_len = ((y_end - y_start) * width) as usize;
            let data = Arc::new(RenderThreadData {
                index: i,
                random_index: i % thread_count,
                exit: AtomicBool::new(false),
                waiting: AtomicBool::new(true),
                done: AtomicBool::new(false),
                x: 0,
                y: y_start,
                x_end: width,
                y_end,
                buffer: Mutex::new(vec![Color::default(); strip_len]),
            });
            
            let data_clone = Arc::clone(&data);
            let scene_clone = Arc::clone(&scene);
            
            thread_datas.push(data);
            threads.push(thread::spawn(move || {
                Self::render_thread(data_clone, scene_clone);
            }));
        }

        Self {
            width,
            height,
            frames,
            movie,
            scene,
            thread_count,
            thread_datas,
            threads,
            threads_alive: true,
        }
    }

    pub fn dispose(&mut self)
    {
        if !self.threads_alive { return; }
        
        for data in &self.thread_datas
        {
            data.exit.store(true, Ordering::Release);
            data.done.store(true, Ordering::Release);
            data.waiting.store(false, Ordering::Release);
        }
        
        for handle in self.threads.drain(..)
        {
            let _ = handle.join();
        }
        
        self.threads_alive = false;
    }

    pub fn render(&mut self, single_threaded: bool) -> &RawMovie
    {
        if single_threaded
        {
            let width_range = (self.width - 1) as Num;
            let height_range = (self.height - 1) as Num;
            
            for f in 0..self.frames
            {
                println!("Frame:{} rendering", f);
                {
                    let mut scene = self.scene.write().unwrap();
                    scene.update_movie_objects(f);
                }
                let scene = self.scene.read().unwrap();

                let mut ray = Ray::default();
                ray.origin = scene.camera.position;

                let colors = &mut self.movie.frames[f as usize].colors;
                
                for y in 0..self.height
                {
                    for x in 0..self.width
                    {
                        ray.direction = Vec3::new(
                            (x as Num / width_range) - 0.5,
                            (y as Num / height_range) - 0.5,
                            -1.0,
                        ).normalize();
                        
                        ray.direction = scene.camera.matrix.transform(ray.direction);

                        // generate shared random vector
                        let random_val = RandomUtil::next_value(x as Num / width_range, y as Num / height_range);
                        let random_vec = Vec3::new(random_val, random_val, random_val) - 0.5;

                        // trace
                        let mut max_bounce = maxBounceCount;
                        let mut light = Color::default();
                        scene.trace(&ray, &mut light, &mut max_bounce, &random_vec);

                        // write result
                        colors[(x + (y * self.width)) as usize] = light;
                    }
                }
            }
        }
        else
        {
            // render frames
            for f in 0..self.frames
            {
                println!("Frame:{} rendering", f);
                {
                    let mut scene = self.scene.write().unwrap();
                    scene.update_movie_objects(f);
                }

                // tell threads to start processing task
                for data in &self.thread_datas
                {
                    data.done.store(false, Ordering::Release);
                    data.waiting.store(false, Ordering::Release);
                }

                // wait for all threads to finish and copy their strips into the frame
                let colors = &mut self.movie.frames[f as usize].colors;
                for data in &self.thread_datas
                {
                    while !data.done.load(Ordering::Acquire)
                    {
                        thread::sleep(Duration::from_millis(1));
                    }
                    
                    let start_idx = (data.y * self.width) as usize;
                    let end_idx = (data.y_end * self.width) as usize;
                    let strip = data.buffer.lock().unwrap();
                    
                    colors[start_idx..end_idx].copy_from_slice(&strip);
                }
            }
        }

        &self.movie
    }

    fn render_thread(data: Arc<RenderThreadData>, scene: Arc<RwLock<Scene>>)
    {
        while !data.exit.load(Ordering::Acquire)
        {
            // wait for main thread to trigger
            while data.waiting.load(Ordering::Acquire)
            {
                thread::sleep(Duration::from_millis(1));
            }
            
            if data.done.load(Ordering::Acquire) || data.exit.load(Ordering::Acquire)
            {
                return;
            }

            {
                let scene = scene.read().unwrap();
                let mut strip = data.buffer.lock().unwrap();

                // setup ray
                let mut ray = Ray::default();
                ray.origin = scene.camera.position;

                // render image section
                let width = scene.width;
                let width_range = (width - 1) as Num;
                let height_range = (scene.height - 1) as Num;
                
                for y in data.y..data.y_end
                {
                    let row_offset = ((y - data.y) * width) as usize;
                    
                    for x in data.x..data.x_end
                    {
                        ray.direction = Vec3::new(
                            (x as Num / width_range) - 0.5,
                            (y as Num / height_range) - 0.5,
                            -1.0,
                        ).normalize();
                        
                        ray.direction = scene.camera.matrix.transform(ray.direction);

                        // generate shared random vector
                        let random_val =
                            RandomUtil::next_value(x as Num / width_range, y as Num / height_range);
                        let random_vec = Vec3::new(random_val, random_val, random_val) - 0.5;

                        // trace
                        let mut max_bounce = maxBounceCount;
                        let mut light = Color::default();
                        scene.trace(&ray, &mut light, &mut max_bounce, &random_vec);

                        // write result
                        strip[row_offset + x as usize] = light;
                    }
                }
            }

            // tell main thread we're done
            data.done.store(true, Ordering::Release);
            data.waiting.store(true, Ordering::Release);
        }
    }
}
