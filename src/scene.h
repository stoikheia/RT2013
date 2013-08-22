/**
 * 
 * @file scene.h
 * @author Reiji Tokuda.
 *
 * Interface file of scene in RT2013 on 2013/06/12.
 * 
 */

#ifndef scene_H
#define scene_H

#include <vector>
#include "shape/geom.hpp"
#include "shape/intersection.h"
#include "material.h"

struct Scene {
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Vertex> vertexes;
    MaterialTable materials;
    std::vector<Vec3> lights;
    
    struct IntersectionInformation {
        Ray ray;
        Vec3 hitpoint;
        Vec3 normal;//surface normal
        real length;//ray to surface
        real angle;//ray to surface
        bool is_triangle;
        size_t geom_id;//index of tiriangles or spheres
    };
    
    const bool get_intersecton(const Ray &ray, IntersectionInformation &info) const {
        info.length = std::numeric_limits<real>::max();
        info.ray = ray;
        return get_intersection_triangles(ray, info) | get_intersection_sperers(ray, info);
    }
    
    bool get_intersection_triangles(const Ray &ray, IntersectionInformation &info) const {
        auto geom_it = triangles.end();
        for (auto it = triangles.begin(); it != triangles.end(); ++it) {
            Vec3 hitpoint, normal;
            real length, angle;
            if(ray.get_intersection_point(*it, vertexes, hitpoint, normal, length, angle)) {
                if(length < info.length) {
                    geom_it = it;
                    info.hitpoint = hitpoint;
                    info.normal = normal;
                    info.length = length;
                    info.angle = angle;
                }
            }
        }
        if(triangles.end() != geom_it) {
            info.is_triangle = true;
            info.geom_id = std::distance(triangles.begin(), geom_it);
            return true;
        } else {
            return false;
        }
    }
    bool get_intersection_sperers(const Ray &ray, IntersectionInformation &info) const {
        auto geom_it = spheres.end();
        for (auto it = spheres.begin(); it != spheres.end(); ++it) {
            std::vector<Vec3> points;
            std::vector<real> lengths;
            if(ray.get_intersection_point(*it, points, lengths)) {
                real length = lengths[0];
                Vec3 hitpoint = points[0];
                if(1 < lengths.size() && lengths[1] <= length) {
                    assert(0 <= lengths[1]);
                    length = lengths[1];
                    hitpoint = points[1];
                }
                if(length < info.length) {
                    geom_it = it;
                    info.hitpoint = hitpoint;
                    info.length = length;
                }
            }
        }
        if(spheres.end() != geom_it) {
            Ray c2p = Ray::create_start_end(geom_it->p, info.hitpoint);
            //diffuse
            info.normal = c2p.n;
            info.angle = ray.n.dot(c2p.n);
            info.is_triangle = false;
            info.geom_id = std::distance(spheres.begin(), geom_it);
            return true;
        } else {
            return false;
        }
    }
    
    bool is_shadow_point(const Ray p2l, const IntersectionInformation &info) const {
        bool shadow_flag = false;
        auto hit_triangle_it = triangles.end();
        auto hit_sphere_it = spheres.end();
        if(info.is_triangle) {
            hit_triangle_it = triangles.begin() + info.geom_id;
        } else {
            hit_sphere_it = spheres.begin() + info.geom_id;
        }
        for (auto it = triangles.begin(); !shadow_flag && it != triangles.end(); ++it) {
            if(it == hit_triangle_it) {
                continue;
            }
            Vec3 point;
            if(p2l.get_intersection_point(*it, vertexes, point)) {
                shadow_flag = true;
            }
        }
        for (auto it = spheres.begin(); !shadow_flag && it != spheres.end(); ++it) {
            if(it == hit_sphere_it) {
                continue;
            }
            std::vector<Vec3> points;
            std::vector<real> lengths;
            if(p2l.get_intersection_point(*it, points, lengths)) {
                shadow_flag = true;
            }
        }
        return shadow_flag;
    }
};

#endif // scene_H
