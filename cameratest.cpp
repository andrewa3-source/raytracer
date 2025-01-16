#include "rtweekend.h"
#include "bvh.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "quad.h"
#include "sphere.h"
#include "camera.h"
#include "constant_medium.h"
#include "material.h"
#include "texture.h"
#include "triangle.h"
#include "trianglemesh.h"
#include "./external/tiny_obj_loader.h"


#define frand() (rand() / (RAND_MAX + 1.0))


void bouncing_spheres(){
    // World

    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    auto tex = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto mat = make_shared<lambertian>(color(.9,.1,.1));
    auto mat2 = make_shared<diffuse_light>(color(7, 7, 7));
    //auto mat = make_shared<lambertian>(color(0.8, 0.8, 0.0));


    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3( 0, random_double(0, .5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;
    cam.render(world);

    std::clog << "\rDone.                 \n";
}

void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker))); 

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);


    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void external_img(){ 
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto earth = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(earth));
}

void perlin_spheres() {
    hittable_list world;

    auto perlin_texture = make_shared<noise_texture>(4);
    auto perlin_surface = make_shared<lambertian>(perlin_texture);
    auto perlin_sphere = make_shared<sphere>(point3(0, 0, 0), 2, perlin_surface);

    world.add(perlin_sphere);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);


    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_smoke() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2), 15),
            vec3(-100,270,395)
        )
    );

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void render_triangle(){
    hittable_list world;

    //auto mat = make_shared<metal>(color(.5, .5, .5), .4);
    auto tex = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto mat = make_shared<lambertian>(tex);
    auto mat2 = make_shared<diffuse_light>(color(7, 7, 7));
    //auto mat = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    world.add(make_shared<TriangleMesh>("./external/dino.obj", mat, point3(-6, 5, 25)));
    world.add(make_shared<sphere>(point3(0, -50, 0), 51, make_shared<dielectric>(1.5)));
    //world.add(make_shared<sphere>(point3(-28, 13, 5), 4, make_shared<dielectric>(1.5)));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1080;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 70;
    cam.lookfrom = point3(-6, 5, 25); // Position the camera at (2, 2, 2)
    cam.lookat   = point3(0, 10, 0); // Look at the center of the bounding box of the triangles
    cam.vup      = vec3(0, 1, 0); // Up direction

    cam.defocus_angle = 0;

    cam.render(world);
}


void render_mario(){
    hittable_list world;

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1920;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0.70,0.80,1.00);

    cam.vfov     = 73;
    cam.lookfrom = point3(200, -200, 200); // Position the camera at (2, 2, 2)
    cam.lookat   = point3(40, 125, 125); // Look at the center of the bounding box of the triangles
    cam.vup      = vec3(0, 0, 1); // Up direction

    cam.defocus_angle = 0;

    auto mat = make_shared<lambertian>(color(.5, .5, .5));
    
    auto mat0 = make_shared<lambertian>(color(1, 1, 0.0));
    world.add(make_shared<TriangleMesh>("./external/model_0.obj", mat0, cam.lookfrom));

    //done
    auto tex1 = make_shared<image_texture>("tex1.png");
    auto mat1 = make_shared<lambertian>(tex1);
    world.add(make_shared<TriangleMesh>("./external/model_1.obj", mat1, cam.lookfrom));

    //done
    auto tex2 = make_shared<checker_texture>(2, color(0.588, 0.588, 0.588), color(0.749, 0.749, 0.749));
    auto mat2 = make_shared<lambertian>(tex2);
    world.add(make_shared<TriangleMesh>("./external/model_2.obj", mat2, cam.lookfrom));


    auto tex3 = make_shared<image_texture>("tex3.png");
    auto mat3 = make_shared<lambertian>(tex3);
    world.add(make_shared<TriangleMesh>("./external/model_3.obj", mat3, cam.lookfrom));

    //done
    auto tex4 = make_shared<image_texture>("tex4.png");
    auto mat4 = make_shared<lambertian>(tex4);
    world.add(make_shared<TriangleMesh>("./external/model_4.obj", mat4, cam.lookfrom, 1));

    //done
    auto tex5 = make_shared<image_texture>("tex5.png");
    auto mat5 = make_shared<lambertian>(tex5);
    world.add(make_shared<TriangleMesh>("./external/model_5.obj", mat5, cam.lookfrom));

    //unsure
    // auto tex6 = make_shared<image_texture>("tex7.png");
    // auto mat6 = make_shared<lambertian>(tex6);
    // world.add(make_shared<TriangleMesh>("./external/model_6.obj", mat6, cam.lookfrom));

    //done
    auto tex7 = make_shared<image_texture>("tex7.png");
    auto mat7 = make_shared<lambertian>(tex7);
    world.add(make_shared<TriangleMesh>("./external/model_7.obj", mat7, cam.lookfrom));

    //done
    auto tex8 = make_shared<image_texture>("tex8.png");
    auto mat8 = make_shared<lambertian>(tex8);
    world.add(make_shared<TriangleMesh>("./external/model_8.obj", mat8, cam.lookfrom, .5));

    //done
    auto tex9 = make_shared<image_texture>("tex9.png");
    auto mat9 = make_shared<lambertian>(tex9);
    world.add(make_shared<TriangleMesh>("./external/model_9.obj", mat9, cam.lookfrom));

    //done
    auto tex10 = make_shared<image_texture>("tex10.png");
    auto mat10 = make_shared<lambertian>(tex10);
    world.add(make_shared<TriangleMesh>("./external/model_10.obj", mat10, cam.lookfrom));

    //unsure
    //world.add(make_shared<TriangleMesh>("./external/model_11.obj", mat, cam.lookfrom));

    //done - box on top
    auto tex12 = make_shared<image_texture>("tex12.png");
    auto mat12 = make_shared<lambertian>(tex12);
    world.add(make_shared<TriangleMesh>("./external/model_12.obj", mat12, cam.lookfrom));

    //done - grass
    auto tex13 = make_shared<image_texture>("tex13.png");
    auto mat13 = make_shared<lambertian>(tex13);
    world.add(make_shared<TriangleMesh>("./external/model_13.obj", mat13, cam.lookfrom));

    //done - chimney
    auto tex14 = make_shared<image_texture>("tex14.png");
    auto mat14 = make_shared<lambertian>(tex14);
    world.add(make_shared<TriangleMesh>("./external/model_14.obj", mat14, cam.lookfrom));

    //done - red roof
    auto tex15 = make_shared<image_texture>("tex15.png");
    auto mat15 = make_shared<lambertian>(tex15);
    world.add(make_shared<TriangleMesh>("./external/model_15.obj", mat15, cam.lookfrom));

    //done - yellow bricks
    auto tex16 = make_shared<image_texture>("tex16.png");
    auto mat16 = make_shared<lambertian>(tex16);
    world.add(make_shared<TriangleMesh>("./external/model_16.obj", mat16, cam.lookfrom));

    //no visible triangles in render
    //world.add(make_shared<TriangleMesh>("./external/model_17.obj", mat, cam.lookfrom));

    //done - fences - might need redone
    auto tex18 = make_shared<image_texture>("tex18.png");
    auto mat18 = make_shared<lambertian>(tex18);
    world.add(make_shared<TriangleMesh>("./external/model_18.obj", mat18, cam.lookfrom));

    //done - water
    auto tex19 = make_shared<image_texture>("tex19.jpg");
    auto mat19 = make_shared<lambertian>(tex19);
    world.add(make_shared<TriangleMesh>("./external/model_19.obj", mat19, cam.lookfrom));

    //done - flower bed sides
    auto tex20 = make_shared<image_texture>("tex20.png");
    auto mat20 = make_shared<lambertian>(tex20);
    world.add(make_shared<TriangleMesh>("./external/model_20.obj", mat20, cam.lookfrom));

    //done flowers
    auto tex21 = make_shared<image_texture>("tex21.png");
    auto mat21 = make_shared<lambertian>(tex21);
    world.add(make_shared<TriangleMesh>("./external/model_21.obj", mat21, cam.lookfrom));

    //done - eyes
    auto tex22 = make_shared<image_texture>("tex22.png");
    auto mat22 = make_shared<lambertian>(tex22);
    world.add(make_shared<TriangleMesh>("./external/model_22.obj", mat22, cam.lookfrom));

    //unsure
    //world.add(make_shared<TriangleMesh>("./external/model_23.obj", mat, cam.lookfrom));

    //done - platforms in back
    auto tex24 = make_shared<image_texture>("tex24.png");
    auto mat24 = make_shared<lambertian>(tex24);
    world.add(make_shared<TriangleMesh>("./external/model_24.obj", mat24, cam.lookfrom));

    //done - wood fence
    auto tex25 = make_shared<image_texture>("tex25.png");
    auto mat25 = make_shared<lambertian>(tex25);
    world.add(make_shared<TriangleMesh>("./external/model_25.obj", mat25, cam.lookfrom));
    // world.add(make_shared<sphere>(point3(0, -50, 0), 51, make_shared<dielectric>(1.5)));
    //world.add(make_shared<sphere>(point3(-28, 13, 5), 4, make_shared<dielectric>(1.5)));

    

    cam.render(world);
}

void render_single_triangle() {
    hittable_list world;

    // Load the texture
    auto texture = make_shared<image_texture>("mmm.png");

    // Create a material with the texture
    auto mat = make_shared<lambertian>(texture);

    // Define the vertices of the triangle
    point3 v0(0, 0, 0);
    point3 v1(100, 0, 0);
    point3 v2(0, 100, 0);

    // Define the normals of the triangle
    vec3 n0(0, 0, 1);
    vec3 n1(0, 0, 1);
    vec3 n2(0, 0, 1);

    // Define the texture coordinates of the triangle
    vec2 t0(0, 0);
    vec2 t1(1, 0);
    vec2 t2(0, 1);

    // Create the triangle
    //auto tri = make_shared<triangle>(v0, v1, v2, n0, n1, n2, t0, t1, t2, mat);
    auto mesh = make_shared<TriangleMesh>("test.obj", mat, point3(0, 0, 2));
    world.add(mesh);

    // Set up the camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 60.0;
    cam.lookfrom = point3(0, 0, 2);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    // Render the scene
    cam.render(world);
}




int main() {
    switch(9){
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
        case 3: external_img(); break;
        case 4: perlin_spheres(); break;
        case 5: quads(); break;
        case 6: simple_light(); break;
        case 7: cornell_box(); break;
        case 8: cornell_smoke(); break;
        case 9: final_scene(1080, 2000, 50); break;
        case 10: render_triangle(); break;
        case 11: render_mario(); break;
        case 12: render_single_triangle(); break;
        default: final_scene(400, 250, 4); break;

    }
}