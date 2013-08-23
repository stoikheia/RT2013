/**
 * 
 * @file geom.hpp
 * @author Reiji Tokuda.
 *
 * Interface file of geom in RT2013 on 2013/06/15.
 * 
 */

#ifndef geom_H
#define geom_H

#include "primitive.hpp"

#include <vector>
#include <algorithm>
#include <cmath>
//#include <exception>
#include <cassert>

//unko macro
#define self (*this)

struct Sphere {
    Vec3 p;
    real r;
    size_t m;//material id
    
	Sphere() {};
    Sphere(const Vec3 &p_, real r_)
    :p(p_),r(r_),m() {}
    Sphere(const Sphere &s)
    :p(s.p),r(s.r),m(s.m) {}
	Sphere& operator=(const Sphere &sphere) {self = sphere;}
	Sphere& operator=(Sphere &&sphere) {self = std::move(sphere);}
};

struct Plane {
    Vec3 n;//normal
    real d;//distance
    
    Plane()
    :n(),d(type_traits<real>::zero()) {}
    Plane(const Vec3 &n_, real d_)
    :n(n_),d(d_) {}
    Plane(const Plane &plane_)
    :n(plane_.n),d(plane_.d) {}
    Plane(Plane &&plane_)
    :n(std::forward<Vec3>(plane_.n)),d(plane_.d) {}
};

struct Triangle {
    
    size_t ids[3];//ids
    
	Triangle() {}
	~Triangle() {}
    Triangle(const Triangle &tri) {memcpy(self.ids,tri.ids, sizeof(ids));}
    Triangle(Triangle &&tri) {memcpy(self.ids, tri.ids, sizeof(ids));}
    Triangle &operator=(const Triangle &tri) {memcpy(self.ids,tri.ids, sizeof(ids));}
    Triangle &operator=(Triangle &&tri) {memcpy(self.ids,tri.ids, sizeof(ids));}
    
    Triangle(size_t id0, size_t id1, size_t id2)
    {ids[0] = id0; ids[1] = id1; ids[2] = id2;}
    Triangle(const size_t *ids_)
    {
        std::copy(ids_, ids_+3, ids);
    }
    
    //utility
    static Plane create_plane(const Vec3 &v0,
                              const Vec3 &v1,
                              const Vec3 &v2) {
        
        const Vec3 &o = v0;
        const Vec3 ov1 = v1 - o;
        const Vec3 ov2 = v2 - o;
        const Vec3 n = ov1.cross(ov2).to_normal();
        real d = -(o.dot(n));
        return Plane(std::move(n), d);
    }
    
    //utility
    template<typename Container>
    Plane create_plane(const Container &vertexes) const {
        //template currying
        return Triangle::create_plane(vertexes[ids[0]].p,
                                      vertexes[ids[1]].p,
                                      vertexes[ids[2]].p);
    }
    
};


#undef self


#endif // geom_H
