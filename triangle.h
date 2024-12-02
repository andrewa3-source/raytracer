#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
    public:
        triangle(const point3& a, const point3& b, const point3& c, shared_ptr<material> mat)
        : a(a), b(b), c(c), mat(mat)
        {
            normal = unit_vector(cross(b - a, c - a));
            D = dot(normal, a);
            w = normal / dot(normal, normal);
            set_bounding_box();
        }
    
        virtual void set_bounding_box() {
            // Compute the bounding box of all three vertices.
            auto bbox_diagonal1 = aabb(a, b);
            auto bbox_diagonal2 = aabb(a, c);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }
    
        aabb bounding_box() const override { return bbox; }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());
    
            // No hit if the ray is parallel to the plane.
            if (std::fabs(denom) < 1e-8)
                return false;
    
            // Return false if the hit point parameter t is outside the ray interval.
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t))
                return false;
    
            // Determine if the intersection point is inside the triangle.
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - a;
            auto alpha = dot(w, cross(planar_hitpt_vector, b - a));
            auto beta = dot(w, cross(c - a, planar_hitpt_vector));
    
            if (!is_interior(alpha, beta, rec))
                return false;
    
            // Ray hits the 2D shape; set the rest of the hit record and return true.
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);
    
            return true;
        }
    
        virtual bool is_interior(double a, double b, hit_record& rec) const {
            interval unit_interval = interval(0, 1);
            // Given the hit point in plane coordinates, return false if it is outside the
            // triangle.
            if (!unit_interval.contains(a) || !unit_interval.contains(b) || a + b > 1)
                return false;
    
            return true;
        }
    
    private:
        point3 a, b, c;
        vec3 normal;
        vec3 w;
        double D;
        shared_ptr<material> mat;
        aabb bbox;
};

#endif