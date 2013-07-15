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
    
    //ray2plane
    bool get_intersection_length(const Plane &plane, real &ret, real &angle) const {
        angle = plane.n.dot(n);
        real distance = o.dot(plane.n);
        
        if(plane.d == -distance) {
            ret = type_traits<real>::zero();
            return true;//both start and end is on the plane
        }
        if(type_traits<real>::zero() == angle) {
            return false;//orthogonal (and not on the plane)
        }
        
        //ret = (-distance - plane.d) / denominator;//memo : similar to formura
        ret = -(distance + plane.d) / angle;
        return true;//intersection (positive or negative)
    }
    bool get_intersection_length(const Plane &plane, real &ret) const {
        real angle;
        return get_intersection_length(plane, ret, angle);
    }
    
    bool get_intersection_point(const Plane &plane, Vec3 &ret, real &length, real &angle) const {
        
        if(!get_intersection_length(plane, length, angle)) {
            return false;//orthogonal and not on the plane
        }
        if(type_traits<real>::zero() <= length && length <= t) {
            //ret = o + n * length;//slow
            ret.x() = o.x() + n.x() * length; ret.y() = o.y() + n.y() * length; ret.z() = o.z() + n.z() * length;//fast
            return true;
        } else {
            return false;
        }
    }
    bool get_intersection_point(const Plane &plane, Vec3 &ret, real &length) const {
        real angle;
        return get_intersection_point(plane, ret, length, angle);
    }
    bool get_intersection_point(const Plane &plane, Vec3 &ret) const {
        real tt;
        return get_intersection_point(plane, ret, tt);
    }
    
    //ray2triangle
    template<typename Conteiner>
    bool get_intersection_point(const Triangle &tri, const Conteiner &vertexes, Vec3 &ret, real &length, real &angle) const {
        if(get_intersection_point(tri.create_plane(vertexes), ret, length, angle)) {
            Vec3 normal;
            if(angle <= 0) {
                normal = self.n * -1.0;
            } else {
                normal = self.n;
            }
            if( (((vertexes[tri.ids[1]].p - vertexes[tri.ids[0]].p).cross(ret - vertexes[tri.ids[0]].p)).dot(normal) >= 0) &&
               (((vertexes[tri.ids[2]].p - vertexes[tri.ids[1]].p).cross(ret - vertexes[tri.ids[1]].p)).dot(normal) >= 0) &&
               (((vertexes[tri.ids[0]].p - vertexes[tri.ids[2]].p).cross(ret - vertexes[tri.ids[2]].p)).dot(normal) >= 0) ) {
                return true;
            }
        }
        return false;
    }
    template<typename Conteiner>
    bool get_intersection_point(const Triangle &tri, const Conteiner &vertexes, Vec3 &ret, real &length) const {
        real direction;
        return get_intersection_point(tri, vertexes, ret, length, direction);
    }
    template<typename Conteiner>
    bool get_intersection_point(const Triangle &tri, const Conteiner &vertexes, Vec3 &ret) const {
        real tt;
        return get_intersection_point(tri, vertexes, ret, tt);
    }
    
    //ray2sphere
    bool get_intersection_point(const Sphere &sphere, std::vector<Vec3> &ret, std::vector<real> &length) const {
        Vec3 op = o - sphere.p;
        real op_dot_n = op.dot(n);
        real op_len = op.length();
        real d = op_dot_n * op_dot_n - (op_len * op_len - sphere.r * sphere.r);
        if(0 <= d) {
            real tt = -op_dot_n - std::sqrt(d);
            if(0 <= tt) {
                length.push_back(tt);
                ret.push_back(o+n*tt);
            }
            tt = -op_dot_n + std::sqrt(d);
            if(0 <= tt) {
                length.push_back(tt);
                ret.push_back(o+n*tt);
            }
            return true;
        } else {
            return false;
        }
    }
    template<typename Conteiner>
    bool get_intersection_point(const Sphere &sphere, std::vector<Vec3> &ret) const {
        std::vector<real> tt;
        return get_intersection_point(sphere, ret, tt);
    }

    //clipping
    bool get_cliped(const Plane &plane, Ray &ret) const {
        
        real tt;
        real direction;
        if(!get_intersection_length(plane, tt, direction)) {//orthogonal
            if(0 < plane.n.dot(o) + plane.d) {
                ret = self;//ray is front of the plane
                return true;
            } else {
                return false;
            }
        }
        
        if(direction >= 0) {//same direction or orthogonal
            if(type_traits<real>::zero() <= tt) {
                if(tt <= t) {
                    ret = Ray(o + n * tt, n, t - tt);//clip : start
                } else {
                    return false;//start/end are behind the plane.
                }
            } else {
                ret = self;//both start and end are front of the plane.
            }
        } else {//opposite direction
            if(type_traits<real>::zero() <= tt) {
                if(tt <= t) {
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
    
    //utilities
    std::pair<Vec3, Vec3> get_points() const {// points of ray starting and end
        return std::pair<Vec3, Vec3>(o, o + n * t);
    }

    static Ray create_start_end(const Vec3 &s, const Vec3 &e) {// creating ray from start and end
        Vec3 dir(e - s);
        real len = dir.length();
        return Ray(s,
                   len!=type_traits<real>::zero()?(dir/len):type_traits<real>::zero(),
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

    AABB(const Sphere &input)
    :min_corner(input.p.x() - input.r, input.p.y() - input.r, input.p.z() - input.r),
    max_corner(input.p.x() + input.r, input.p.y() + input.r, input.p.z() + input.r) {}
    
    template<typename Conteiner>
    AABB(const Triangle &tri, const Conteiner &vertexes) {
        
        static_assert(std::is_same<typename Conteiner::iterator::iterator_category, std::random_access_iterator_tag>::value,
                      "AABB triangle initializer doesnt accept if it is not random access iterator.");
        
        for(size_t i = 0; i < 3; ++i) {//xyz
            self.min_corner.e[i] = std::min(vertexes[tri.ids[0]].p[i],
                                            std::min(vertexes[tri.ids[1]].p[i],
                                                     vertexes[tri.ids[2]].p[i]));
            self.max_corner.e[i] = std::max(vertexes[tri.ids[0]].p[i],
                                            std::max(vertexes[tri.ids[1]].p[i],
                                                     vertexes[tri.ids[2]].p[i]));
        }
    }

    void merge(const AABB &input) {
        for(size_t i = 0; i < 3; ++i) {
            self.min_corner.e[i] = std::min(self.min_corner.e[i], input.min_corner.e[i]);
            self.max_corner.e[i] = std::max(self.max_corner.e[i], input.max_corner.e[i]);
        }
    }

    void create_edges(std::vector<Ray> &ret) const;
    
    bool get_intersection_points(const Plane &plane, std::vector<Vec3> &ret) const;
    
    //triangle
    template<typename Conteiner>
    bool does_intersect(const Triangle &tri, const Conteiner &vertexes) const {
        //template currying
        return does_intersect(vertexes[tri.ids[0]].p, vertexes[tri.ids[1]].p, vertexes[tri.ids[2]].p);
    }
    //point
    bool does_intersect(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) const;
    //ray
    bool does_intersect(const Ray &ray) const;
};



#undef self

#endif // collision_H
