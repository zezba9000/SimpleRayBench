#![allow(dead_code)]

use crate::benchmark::{Num, RawImage, RawMovie};
use std::fs::File;
use std::io::{BufWriter, Write};

pub struct Utils;

impl Utils {
    pub fn convert_raw_image_sdr(image: &RawImage) -> Vec<u8> {
        let image_data_length = (image.width * image.height * 3) as usize;
        let mut data = vec![0u8; image_data_length];
        let mut i = 0;
        // for (int y = 0; y != image.height; ++y)
        for y in (0..image.height).rev() {
            // invert Y
            for x in 0..image.width {
                let idx = (x + (y * image.width)) as usize;
                let color = image.colors[idx] * (255.0 as Num);
                data[i] = color.r.clamp(0.0, 255.0) as u8;
                data[i + 1] = color.g.clamp(0.0, 255.0) as u8;
                data[i + 2] = color.b.clamp(0.0, 255.0) as u8;
                i += 3;
            }
        }
        data
    }

    pub fn write_frame_ppm<W: Write>(stream: &mut W, width: i32, height: i32, image_data: &[u8]) {
        // PPM P6 binary header
        let header = format!("P6\n{} {}\n255\n", width, height);
        stream.write_all(header.as_bytes()).unwrap();

        // RGB pixel data
        stream.write_all(image_data).unwrap();
    }

    pub fn save_image_data_ppm(filename: &str, image: &RawImage) {
        // convert image to SDR data
        let image_data = Self::convert_raw_image_sdr(image);

        // save to file
        let file = File::create(filename).unwrap();
        let mut stream = BufWriter::new(file);

        // write frame
        Self::write_frame_ppm(&mut stream, image.width, image.height, &image_data);

        // finish
        stream.flush().unwrap();
    }

    pub fn save_movie_data_ppm(filename: &str, movie: &RawMovie) {
        // save to file
        let file = File::create(filename).unwrap();
        let mut stream = BufWriter::new(file);

        for i in 0..movie.frame_count as usize {
            let image = &movie.frames[i];

            // convert image to SDR data
            let image_data = Self::convert_raw_image_sdr(image);

            // write frame
            Self::write_frame_ppm(&mut stream, image.width, image.height, &image_data);
        }

        // finish
        stream.flush().unwrap();
    }
}
