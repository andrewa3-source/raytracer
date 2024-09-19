#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vec3.h"
#include "color.h"
#include "ray.h"

#define X 2560
#define Y 1440

#define frand() (rand() / (RAND_MAX + 1.0))

// typedef double vec3[3];

typedef struct{
    vec3 a, b, c;
} triangle;

//int8_t img[Y][X][3];

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), .5, r))
            return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]){
    FILE *f;
    triangle t;
    int x, y, i;
    vec3 p, q;
    vec3 n, na, nb, nc;
    vec3 d1, d2;
    vec3 bary;
    vec3 colorf;
    int aa = 1;

    //Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    int8_t img[image_height][image_width][3];

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    if (argc == 2) {
        aa = atoi(argv[1]);
    }
    t.a[0] = 0.1;
    t.a[1] = 0.1;
    t.a[2] = 0.0;

    t.b[0] = 0.8;
    t.b[1] = 0.2;
    t.b[2] = 0.0;

    t.c[0] = 0.5;
    t.c[1] = 0.9;
    t.c[2] = 0.0;

    p[2] = q[2] = 0;

    /// n = (b - a) x (c - a)
    d1 = t.b - t.a;
    d2 = t.c - t.a;
    n = cross(d1, d2);

    // vec3_sub(d1, t.b, t.a);
    // vec3_sub(d2, t.c, t.a);
    // vec3_cross(n, d1, d2);

    for (y = 0; y < image_height; y++) {
        p[1] = ((double)y / Y);
        for(x = 0; x < image_width; x++){
            colorf[0] = colorf[1] = colorf[2] = 0;
            p[0] = ((double)x / X);

            auto pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(img, x, y, pixel_color);

            // for (i = 0; i < aa; i++){
            //     q[0] = p[0] - frand() * (1.0 / X);
            //     q[1] = p[1] - frand() * (1.0 / Y);
        
            //     //na = (c - b) x (p - b)
            //     d1 = t.c - t.b;
            //     d2 = q - t.b;
            //     na = cross(d1, d2);
            //     // vec3_sub(d1, t.c, t.b);
            //     // vec3_sub(d2, q, t.b);
            //     // vec3_cross(na, d1, d2);

            //     //nb = (a - c) x (p - c)
            //     d1 = t.a - t.c;
            //     d2 = q - t.c;
            //     nb = cross(d1, d2);
            //     // vec3_sub(d1, t.a, t.c);
            //     // vec3_sub(d2, q, t.c);
            //     // vec3_cross(nb, d1, d2);

            //     //nc = (b - a) x (p - a)
            //     d1 = t.b - t.a;
            //     d2 = q - t.a;
            //     nc = cross(d1, d2);
            //     // vec3_sub(d1, t.b, t.a);
            //     // vec3_sub(d2, q, t.a);
            //     // vec3_cross(nc, d1, d2);

            //     bary[0] = dot(n, na) / dot(n, n);
            //     bary[1] = dot(n, nb) / dot(n, n);
            //     bary[2] = dot(n, nc) / dot(n, n);

            //     // bary[0] = vec3_dot(n, na) / vec3_dot(n, n);
            //     // bary[1] = vec3_dot(n, nb) / vec3_dot(n, n);
            //     // bary[2] = vec3_dot(n, nc) / vec3_dot(n, n);
            //     if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0){
            //         colorf[0] += bary[0];
            //         colorf[1] += bary[1];
            //         colorf[2] += bary[2];
            //     }
            // }
            // write_color(img, x, y, colorf / aa);
            // // img[y][x][0] = color[0] * 255 / aa;
            // // img[y][x][1] = color[1] * 255 / aa;
            // // img[y][x][2] = color[2] * 255 / aa;
            
        }
    }

    

    f = fopen("barycentric.ppm", "w");
    //P6 -> X -> Y -> 255(color depth)
    fprintf(f, "P3\n%d\n%d\n255\n", X, Y);
    fwrite(img, 3, image_width * image_height, f);
    fclose(f);
}