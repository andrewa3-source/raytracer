#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class perlin {
    public:
        perlin() {
            for (int i = 0; i < point_count; i++) {
                randvec[i] = unit_vector(vec3::random(-1, 1));
            }

            perlin_generate_perm(permx);
            perlin_generate_perm(permy);
            perlin_generate_perm(permz);
        }
        
        double noise(const point3& p) const {
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            auto i = int(floor(p.x()));
            auto j = int(floor(p.y()));
            auto k = int(floor(p.z()));

            vec3 c[2][2][2];

            for (int di=0; di < 2; di++) {
                for (int dj=0; dj < 2; dj++) {
                    for (int dk=0; dk < 2; dk++) {
                        c[di][dj][dk] = randvec[
                            permx[(i+di) & 255] ^
                            permy[(j+dj) & 255] ^
                            permz[(k+dk) & 255]
                        ];
                    }
                }
            }

            return perlin_interp(c, u, v, w);
        }

        double turb(const point3& p, int depth=7) const {
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }

            return fabs(accum);
        }

    private:
        static const int point_count = 256;
        vec3 randvec[point_count];
        int permx[point_count];
        int permy[point_count];
        int permz[point_count];

        static void perlin_generate_perm(int* p) {
            for (int i = 0; i < point_count; i++) {
                p[i] = i;
            }

            permute(p, point_count);
        }

        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
            auto accum = 0.0;
            for (int i=0; i < 2; i++) {
                for (int j=0; j < 2; j++) {
                    for (int k=0; k < 2; k++) {
                        accum += (i*u + (1-i)*(1-u)) *
                                 (j*v + (1-j)*(1-v)) *
                                 (k*w + (1-k)*(1-w)) * c[i][j][k];
                    }
                }
            }
            return accum;
        }

        static double perlin_interp(const vec3 c[2][2][2], double u, double v, double w) {
            auto uu = u*u*(3-2*u);
            auto vv = v*v*(3-2*v);
            auto ww = w*w*(3-2*w);
            auto accum = 0.0;

            for (int i=0; i < 2; i++) {
                for (int j=0; j < 2; j++) {
                    for (int k=0; k < 2; k++) {
                        vec3 weight_v(u-i, v-j, w-k);
                        accum += (i*uu + (1-i)*(1-uu)) *
                                 (j*vv + (1-j)*(1-vv)) *
                                 (k*ww + (1-k)*(1-ww)) * dot(c[i][j][k], weight_v);
                    }
                }
            }
            return accum;
        }
};

#endif