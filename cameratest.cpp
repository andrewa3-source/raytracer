#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#define frand() (rand() / (RAND_MAX + 1.0))


int main() {
    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    world.add(make_shared<sphere>(point3(1,0,-1), 0.25));

    // Camera
    camera cam;

    cam.ascpect_ratio = 16.0 / 9.0;
    cam.image_width = 400;

    cam.render(world);

    std::clog << "\rDone.                 \n";
}