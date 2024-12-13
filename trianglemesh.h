#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <cfloat> // Include for FLT_MAX
#include <iostream> // Include for debug output
#include <vector>
#include "hittable.h"
#include "triangle.h"
#include "aabb.h"
#include "material.h"
#include "vec3.h"
#include "interval.h"
#include "external/tiny_obj_loader.h"

class TriangleMesh : public hittable {
public:
    std::vector<shared_ptr<triangle>> triangles;

    TriangleMesh(const std::string& filename, shared_ptr<material> mat, const point3& camera_position, double texture_scale = 1.0) {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, filename.c_str())) {
            std::cerr << "Failed to load/parse .obj file: " << filename << std::endl;
            return;
        }

        // Calculate the bounding box of the entire mesh
        point3 min_point(FLT_MAX, FLT_MAX, FLT_MAX);
        point3 max_point(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                tinyobj::real_t vx = attributes.vertices[3 * index.vertex_index + 0];
                tinyobj::real_t vy = attributes.vertices[3 * index.vertex_index + 1];
                tinyobj::real_t vz = attributes.vertices[3 * index.vertex_index + 2];

                min_point = point3(fmin(min_point.x(), vx), fmin(min_point.y(), vy), fmin(min_point.z(), vz));
                max_point = point3(fmax(max_point.x(), vx), fmax(max_point.y(), vy), fmax(max_point.z(), vz));
            }
        }

        vec3 bbox_size = max_point - min_point;

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
                    tinyobj::real_t vx = attributes.vertices[3 * idx.vertex_index + 0];
                    tinyobj::real_t vy = attributes.vertices[3 * idx.vertex_index + 1];
                    tinyobj::real_t vz = attributes.vertices[3 * idx.vertex_index + 2];
                    vertices.push_back(point3(vx, vy, vz));

                    // Check if normal index is valid
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attributes.normals[3 * idx.normal_index + 0];
                        tinyobj::real_t ny = attributes.normals[3 * idx.normal_index + 1];
                        tinyobj::real_t nz = attributes.normals[3 * idx.normal_index + 2];
                        normals.push_back(vec3(nx, ny, nz));
                    } else {
                        normals.push_back(vec3(0, 0, 0)); // Default normal if not provided
                    }

                    // Check if texture coordinate index is valid
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attributes.texcoords[2 * idx.texcoord_index + 0];
                        tinyobj::real_t ty = attributes.texcoords[2 * idx.texcoord_index + 1];
                        // Normalize texture coordinates based on the bounding box of the mesh and apply scaling
                        //tx = ((vertices[v].x() - min_point.x()) / bbox_size.x()) * texture_scale;
                        //ty = ((vertices[v].y() - min_point.y()) / bbox_size.y()) * texture_scale;
                        texcoords.push_back(vec2(tx, ty));
                    } else {
                        // Generate texture coordinates based on vertex positions if not provided
                        //tinyobj::real_t tx = ((vertices[v].x() - min_point.x()) / bbox_size.x()) * texture_scale;
                        //tinyobj::real_t ty = ((vertices[v].y() - min_point.y()) / bbox_size.y()) * texture_scale;
                        texcoords.push_back(vec2(0, 0));
                    }
                }
                index_offset += fv;

                if (vertices.size() == 3) {
                    vec3 normal = unit_vector(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
                    // Check if the normal is facing the camera
                    if (dot(normal, vertices[0] - camera_position) < 0) {
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
    }

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& tri : triangles) {
            if (tri->hit(r, interval(0, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
                //std::cout << "Hit triangle at t = " << temp_rec.t << ", point = " << temp_rec.p << ", normal = " << temp_rec.normal << std::endl;
            
            }
        }

        return hit_anything;
    }

    virtual aabb bounding_box() const override {
        if (triangles.empty()) return aabb();

        aabb output_box = triangles[0]->bounding_box();
        for (const auto& tri : triangles) {
            output_box = surrounding_box(output_box, tri->bounding_box());
        }

        return output_box;
    }
    
};

#endif