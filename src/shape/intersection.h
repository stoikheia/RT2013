/**
 * 
 * @file intersection.h
 * @author Reiji Tokuda.
 *
 * Interface file of collision in RT2013 on 2013/06/15.
 * 
 */

#ifndef collision_H
#define collision_H

#include "geom.hpp"

//unko macro
#define self (*this)

struct Ray {
    Vec3 o,n;//origin, direction(normal)
    real t;//length

    Ray() = default;
    Ray(const Ray&) = default;
    Ray(Ray&&) = default;
    ~Ray() = default;
    Ray& operator=(const Ray&) = default;
    Ray& operator=(Ray&&) = default;
    
    Ray(const Vec3 &o_, const Vec3 &n_, real len = type_traits<real>::inf())
    :o(o_),n(n_),t(len) {}
    
    bool operator==(const Ray &ray) const {
        return 0 == memcmp(this, &ray, sizeof(Ray));
    }
    
    bool get_intersection_length(const Plane &plane, real &ret) const {
        real denominator = plane.n.dot(n);
        real distance = o.dot(plane.n);
        
        if(plane.d == -distance) {
            ret = type_traits<real>::zero();
            return true;//both start and end is on the plane
        }
        if(type_traits<real>::zero() == denominator) {
            return false;//orthogonal (and not on the plane)
        }
        
        //ret = (-distance - plane.d) / denominator;//memo : similar to formura
        ret = -(distance + plane.d) / denominator;
        return true;//intersection (positive or negative)
    }

    bool get_intersection_point(const Plane &plane, Vec3 &ret) const {

        real tt;
        if(!get_intersection_length(plane, tt)) {
            return false;//orthogonal
        }
        if(type_traits<real>::zero() <= tt && tt <= t) {
            ret.x() = o.x() + n.x() * tt; ret.y() = o.y() + n.y() * tt; ret.z() = o.z() + n.z() * tt;
            return true;
        } else {
            return false;
        }
    }

    bool get_cliped(const Plane &plane, Ray &ret) const {
        
        real tt;
        if(!get_intersection_length(plane, tt)) {//orthogonal
            if(0 < plane.n.dot(o) + plane.d) {
                ret = self;//ray is front of the plane
                return true;
            } else {
                return false;
            }
        }
        
        real dir = plane.n.dot(n);
        if(type_traits<real>::zero() <= dir) {//same direction
            if(type_traits<real>::zero() <= tt) {
                if(tt < t) {
                    ret = Ray(o + n * tt, n, t - tt);//clip : start
                } else {
                    return false;//start/end are behind the plane.
                }
            } else {
                ret = self;//both start and end are front of the plane.
            }
        } else {//opposite direction
            if(type_traits<real>::zero() <= tt) {
                if(tt < t) {
                    ret = Ray(o, n, tt);//clip : end
                } else {
                    ret = self;//start/end are front of the plane.
                }
            } else {
                return false;//start/end are behind the plane.
            }
        }
        return true;
    }
    
    std::pair<Vec3, Vec3> get_points() const {
        return std::pair<Vec3, Vec3>(o, o + n * t);
    }

    static Ray create_start_end(const Vec3 &s, const Vec3 &e) {
        Vec3 dir(e - s);
        real len = dir.length();
        return Ray(s,
                   len!=type_traits<real>::zero()?(dir/len):s,
                   len!=type_traits<real>::zero()?len:type_traits<real>::inf());
    }
};



struct AABB {

    Vec3 min_corner;
    Vec3 max_corner;

    AABB() = default;
    AABB(const AABB &) = default;
    AABB(AABB &&) = default;
    ~AABB() = default;
    AABB& operator=(const AABB&) = default;
    AABB& operator=(AABB&&) = default;
    
    AABB(const Vec3 &input_min_corner, const Vec3 &input_max_corner)
        :min_corner(input_min_corner),max_corner(input_max_corner) {}

    template<typename Conteiner>
    AABB(const Triangle &tri, const Conteiner &vertexes) {
        static_assert(std::is_same<typename Conteiner::iterator::iterator_category, std::random_access_iterator_tag>::value,
                "AABB triangle initializer doesnt accept if it is not random access iterator.");
        min_corner = vertexes[tri.ids[0]].p; max_corner = vertexes[tri.ids[0]].p;
        for(size_t i = 1; i < 3; ++i) {
            for(size_t j = 0; j < 3; ++j) {//xyz
                min_corner[j] = std::min(min_corner[j], vertexes[tri.ids[i]].p[j]);
                max_corner[j] = std::max(max_corner[j], vertexes[tri.ids[i]].p[j]);
            }
        }
    }

    AABB(const Sphere &input)
    :min_corner(input.p.x() - input.r, input.p.y() - input.r, input.p.z() - input.r),
    max_corner(input.p.x() + input.r, input.p.y() + input.r, input.p.z() + input.r) {}

    void merge(const AABB &input) {
        for(size_t i = 0; i < 3; ++i) {
            self.min_corner.e[i] = std::min(self.min_corner.e[i], input.min_corner.e[i]);
            self.max_corner.e[i] = std::max(self.max_corner.e[i], input.max_corner.e[i]);
        }
    }

    void create_edges(std::vector<Ray> &ret) const;
    
    bool get_intersection_points(const Plane &plane, std::vector<Vec3> &ret) const;

    template<typename Conteiner>
    bool does_intersect(const Triangle &tri, const Conteiner &vertexes) const {
        //template currying
        return does_intersect(vertexes[tri.ids[0]].p, vertexes[tri.ids[1]].p, vertexes[tri.ids[2]].p);
    }
    bool does_intersect(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) const;
};



#undef self

#endif // collision_H
