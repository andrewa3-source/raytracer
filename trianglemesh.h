#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "hittable.h"
#include "hittable_list.h"
#include "vec3.h"
#include "triangle.h"
#include "./external/tiny_obj_loader.h"
#include <fstream>
#include <sstream>
//need to be able to use make_shared triangle
#include "rtweekend.h"


class TriangleMesh : public hittable {
public:
    std::vector<shared_ptr<triangle>> triangles;

    TriangleMesh(const std::string& filename, shared_ptr<material> mat) {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, filename.c_str())) {
            std::cerr << "Failed to load/parse .obj file: " << filename << std::endl;
            return;
        }

        for (const auto& shape : shapes) {
            size_t index_offset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                int fv = shape.mesh.num_face_vertices[f];
                
                std::vector<point3> vertices;
                std::vector<vec3> normals;
                std::vector<vec2> texcoords;
                
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attributes.vertices[3*idx.vertex_index+0];
                    tinyobj::real_t vy = attributes.vertices[3*idx.vertex_index+1];
                    tinyobj::real_t vz = attributes.vertices[3*idx.vertex_index+2];
                    vertices.push_back(point3(vx, vy, vz));

                    // Check if normal index is valid
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attributes.normals[3*idx.normal_index+0];
                        tinyobj::real_t ny = attributes.normals[3*idx.normal_index+1];
                        tinyobj::real_t nz = attributes.normals[3*idx.normal_index+2];
                        normals.push_back(vec3(nx, ny, nz));
                    } else {
                        normals.push_back(vec3(0, 0, 0)); // Default normal if not provided
                    }

                    // Check if texture coordinate index is valid
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attributes.texcoords[2*idx.texcoord_index+0];
                        tinyobj::real_t ty = attributes.texcoords[2*idx.texcoord_index+1];
                        texcoords.push_back(vec2(tx, ty));
                    } else {
                        texcoords.push_back(vec2(0, 0)); // Default texcoord if not provided
                    }
                }
                index_offset += fv;

                if (vertices.size() == 3) {
                    auto tri = make_shared<triangle>(
                        vertices[0], vertices[1], vertices[2],
                        normals[0], normals[1], normals[2],
                        texcoords[0], texcoords[1], texcoords[2],
                        mat
                    );
                    triangles.push_back(tri); // Ensure the triangle is added to the vector
                }
            }
        }
    }

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& tri : triangles) {
            if (tri->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    virtual aabb bounding_box() const override {
        if (triangles.empty()) return aabb::empty;

        aabb output_box;
        bool first_box = true;

        for (const auto& tri : triangles) {
            aabb tri_box = tri->bounding_box();
            if (!tri_box.valid()) return aabb::empty;
            output_box = first_box ? tri_box : surrounding_box(output_box, tri_box);
            first_box = false;
        }

        return output_box;
    }
};

#endif