#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#define frand() (rand() / (RAND_MAX + 1.0))

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
        //return (color(1, 0, 0));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    vec3 p, q;
    int aa = 100;
    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    world.add(make_shared<sphere>(point3(1,0,-1), 0.25));

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
    std::clog << "pixel_delta_u: " << pixel_delta_u << std::endl;   
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    color pixel_color;
    for (int j = 0; j < image_height; j++) {
        p[1] = ((double)j / image_height); 
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            p[0] = ((double)i / image_width);
            pixel_color = color(0, 0, 0);
            for (int s = 0; s < aa; s++) {
                vec3 adjust_u = pixel_delta_u + vec3(frand() * (0.05 / image_width), 0, 0);
                vec3 adjust_v = pixel_delta_v + vec3(0, frand() * (0.05 / image_height), 0); 
                
                double i_adj = i - frand() ;
                double j_adj = j - frand() ;
                // std::clog << "i: " << i << std::endl;
                // std::clog << "i_adj: " << i_adj << std::endl;
                //std::clog << "adjust_u: " << adjust_u << std::endl;
                //std::clog << "q: " << q << std::endl;
                auto pixel_center = pixel00_loc + (i_adj * pixel_delta_u) + (j_adj * pixel_delta_v);
                //print the pixel center
                //std::clog << "pixel_center: " << pixel_center << std::endl;
                auto ray_direction = pixel_center - camera_center;
                ray r(camera_center, ray_direction);

                pixel_color += ray_color(r, world);
                //make this print 
                // std::clog << "pixel_center: " << pixel_center << std::endl;
                // std::clog << "ray_direction: " << ray_direction << std::endl;
                // std::clog << "r: " << r << std::endl;
                //std::clog << "pixel_color: " << pixel_color << std::endl;
                //printf("pixel_color: %f %f %f\n", pixel_color[0], pixel_color[1], pixel_color[2]);
            }
            pixel_color /= aa;
            //std::clog << "pixel_color: " << pixel_color << std::endl;
            write_color(std::cout, pixel_color);
            // auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            // auto ray_direction = pixel_center - camera_center;
            // ray r(camera_center, ray_direction);

            // color pixel_color = ray_color(r, world);
            // write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}