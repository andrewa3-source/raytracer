#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "vec2.h"

struct Vertex {
public:
    vec3 P;
    vec3 Ng;
    vec3 UV;
};

class triangle : public hittable {
public:
    point3 a, b, c;
    vec3 normal_a, normal_b, normal_c;
    vec2 texcoord_a, texcoord_b, texcoord_c;
    shared_ptr<material> mat;

    triangle(const point3& a, const point3& b, const point3& c,
             const vec3& normal_a, const vec3& normal_b, const vec3& normal_c,
             const vec2& texcoord_a, const vec2& texcoord_b, const vec2& texcoord_c,
             shared_ptr<material> mat)
        : a(a), b(b), c(c),
          normal_a(normal_a), normal_b(normal_b), normal_c(normal_c),
          texcoord_a(texcoord_a), texcoord_b(texcoord_b), texcoord_c(texcoord_c),
          mat(mat) {
        normal = unit_vector(cross(b - a, c - a));
    }

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    // Möller–Trumbore intersection algorithm
    const auto epsilon = 1e-8;
    vec3 edge1 = b - a;
    vec3 edge2 = c - a;
    vec3 h = cross(r.direction(), edge2);
    auto det = dot(edge1, h);
    if (det > -epsilon && det < epsilon) return false; // This ray is parallel to this triangle.

    auto inv_det = 1.0 / det;
    vec3 s = r.origin() - a;
    auto u = inv_det * dot(s, h);
    if (u < 0.0 || u > 1.0) return false;

    vec3 q = cross(s, edge1);
    auto v = inv_det * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0) return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    auto t = inv_det * dot(edge2, q);
    if (t > epsilon && t < ray_t.max && t > ray_t.min) { // ray intersection within the interval
        rec.t = t;
        rec.p = r.at(t);
        rec.normal = unit_vector((1 - u - v) * normal_a + u * normal_b + v * normal_c);
        rec.mat = mat;
        rec.u = (1 - u - v) * texcoord_a.x() + u * texcoord_b.x() + v * texcoord_c.x();
        rec.v = (1 - u - v) * texcoord_a.y() + u * texcoord_b.y() + v * texcoord_c.y();
        return true;
    } else // This means that there is a line intersection but not a ray intersection within the interval.
        return false;
}

    virtual aabb bounding_box() const override {
        point3 min_point(
            std::min({a.x(), b.x(), c.x()}),
            std::min({a.y(), b.y(), c.y()}),
            std::min({a.z(), b.z(), c.z()})
        );
        point3 max_point(
            std::max({a.x(), b.x(), c.x()}),
            std::max({a.y(), b.y(), c.y()}),
            std::max({a.z(), b.z(), c.z()})
        );
        return aabb(min_point, max_point);
    }

private:
    vec3 normal;
    aabb bbox;
};


#endif