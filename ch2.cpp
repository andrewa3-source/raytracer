#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vec3.h"

#define X 2560
#define Y 1440

#define frand() (rand() / (RAND_MAX + 1.0))

// typedef double vec3[3];

typedef struct{
    vec3 a, b, c;
} triangle;

int8_t img[Y][X][3];

void vec3_cross(vec3 c, vec3 v, vec3 w){
    c[0] = v[1] * v[2] - w[1] * v[2];
    c[1] = v[2] * w[0] - w[2] * v[0];
    c[2] = v[0] * w[1] - w[0] * v[1];
}

double vec3_dot(vec3 v, vec3 w){
    return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
}

void vec3_sub(vec3 s, vec3 v, vec3 w){
    s[0] = v[0] - w[0];
    s[1] = v[1] - w[1];
    s[2] = v[2] - w[2];
}  

int main(int argc, char *argv[]){
    FILE *f;
    triangle t;
    int x, y, i;
    vec3 p, q;
    vec3 n, na, nb, nc;
    vec3 d1, d2;
    vec3 bary;
    vec3 color;
    int aa = 1;

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
    //vec3_cross(n, d1, d2);

    printf("n: %f %f %f\n", n[0], n[1], n[2]);
    for (y = 0; y < Y; y++) {
        p[1] = ((double)y / Y);
        for(x = 0; x < X; x++){
            color[0] = color[1] = color[2] = 0;
            p[0] = ((double)x / X);

            for (i = 0; i < aa; i++){
                q[0] = p[0] - frand() * (1.0 / X);
                q[1] = p[1] - frand() * (1.0 / Y);
        
                //na = (c - b) x (p - b)
                d1 = t.c - t.b;
                d2 = q - t.b;
                na = cross(d1, d2);
                // vec3_sub(d1, t.c, t.b);
                // vec3_sub(d2, q, t.b);
                // vec3_cross(na, d1, d2);

                //nb = (a - c) x (p - c)
                d1 = t.a - t.c;
                d2 = q - t.c;
                nb = cross(d1, d2);
                // vec3_sub(d1, t.a, t.c);
                // vec3_sub(d2, q, t.c);
                // vec3_cross(nb, d1, d2);

                //nc = (b - a) x (p - a)
                d1 = t.b - t.a;
                d2 = q - t.a;
                nc = cross(d1, d2);
                // vec3_sub(d1, t.b, t.a);
                // vec3_sub(d2, q, t.a);
                // vec3_cross(nc, d1, d2);

                bary[0] = dot(n, na) / dot(n, n);
                bary[1] = dot(n, nb) / dot(n, n);
                bary[2] = dot(n, nc) / dot(n, n);

                // bary[0] = vec3_dot(n, na) / vec3_dot(n, n);
                // bary[1] = vec3_dot(n, nb) / vec3_dot(n, n);
                // bary[2] = vec3_dot(n, nc) / vec3_dot(n, n);
                if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0){
                    color[0] += bary[0];
                    color[1] += bary[1];
                    color[2] += bary[2];
                }
            }
            img[y][x][0] = color[0] * 255 / aa;
            img[y][x][1] = color[1] * 255 / aa;
            img[y][x][2] = color[2] * 255 / aa;
            
        }
    }

    

    f = fopen("barycentric.ppm", "w");
    //P6 -> X -> Y -> 255(color depth)
    fprintf(f, "P6\n%d\n%d\n255\n", X, Y);
    fwrite(img, 3, X * Y, f);
    fclose(f);
}