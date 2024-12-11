#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "color.h"
#include "rtw_stb_image.h"
#include "perlin.h"

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const color& albedo) : albedo(albedo) {}

        solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}
        color value(double u, double v, const point3& p) const override {
            return albedo;
        }
    
    private:
        color albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

        checker_texture(double scale, const color& c1, const color& c2)
            : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

        color value(double u, double v, const point3& p) const override {
            auto xInt = int(std::floor(inv_scale * p.x()));
            auto yInt = int(std::floor(inv_scale * p.y()));
            auto zInt = int(std::floor(inv_scale * p.z()));

            bool isEven = (xInt + yInt + zInt) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }
    private:
        double inv_scale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;    
};

class image_texture : public texture {
public:
    image_texture(const char* filename) {
        auto components_per_pixel = 3;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = components_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual color value(double u, double v, const point3& p) const override {
        if (data == nullptr) return color(0, 1, 1); // Return cyan if no texture data

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width)  i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * 3;

        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};

class noise_texture : public texture {
    public:
        noise_texture(double scale) : scale(scale), col(color(.5,.5,.5)) {}
        noise_texture(double scale, color col) : scale(scale), col(col) {}
        
        color value(double u, double v, const point3& p) const override {
            //return color(1,1,1) * 0.5 * (1.0 + noise.noise(scale * p));
            //return color(1,1,1) * noise.turb(p, 7);
            return col * (1 + sin(scale * p.z() + 10 * noise.turb(p, 7)));
        }
    
    private: 
        perlin noise;
        double scale;
        color col;
};



#endif