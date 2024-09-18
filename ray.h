#ifndef RAY_H 
#define RAY_H

#include "vec3.h"

class ray {
  public:
    ray() {}

    //Ray constructor - point of origin and vector direction
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    //Getters
    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    //Ray function - returns a point along the ray at given time t
    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif