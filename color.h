#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#define X 2560
#define Y 1440

using color = vec3;

void write_color(int8_t img[Y][X][3], const color &pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //translate the [0,1] component values to the byte range [0,255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g); 
    int bbyte = int(255.999 * b);

    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            img[y][x][0] = rbyte;
            img[y][x][1] = gbyte;
            img[y][x][2] = bbyte;
        }
    } 
     
    
}

#endif