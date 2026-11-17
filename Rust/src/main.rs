#![forbid(unsafe_code)]

mod benchmark;
mod utils;

use benchmark::Benchmark;
use std::time::Instant;
use utils::Utils;

fn main() {
    const SIZE: i32 = 512; // image size
    let mut bench = Benchmark::init(SIZE, SIZE, 200);

    let start = Instant::now();
    bench.render(false); // run benchmark
    let sec = start.elapsed().as_secs_f64();
    println!("Render Time: {}s", sec);

    Utils::save_movie_data_ppm("image.ppm", &bench.movie);
    bench.dispose();
}
