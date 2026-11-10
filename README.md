# Simple Raytracing Benchmark
Tests different languages or system performance.<br>
This is not a micro-benchmark and thus better tests longer running situations

## What this does
Renders a short raw video to image.ppm on console platforms<br>
Some platforms may instead render to textures/images to show results

### Convert Raw Video to mp4
* Download FFmpeg: https://ffmpeg.org/download.html?utm_source=chatgpt.com
* Run command: ffmpeg -f image2pipe -framerate 10 -vcodec ppm -i \<input-path-to-image.ppm\> -c:v libx264 -pix_fmt yuv420p <output-path-to-movie.mp4>