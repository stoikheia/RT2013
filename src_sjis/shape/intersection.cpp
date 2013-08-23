/**
 * 
 * @file collision.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of collision in RT2013 on 2013/06/15.
 * 
 */


#include "intersection.h"

#include "geom_util.h"

#include <vector>

void AABB::create_edges(std::vector<Ray> &ret) const {
    static const Vec3 normals[] = {
        Vec3(type_traits<real>::one(),
             type_traits<real>::zero(),
             type_traits<real>::zero()),
        Vec3(type_traits<real>::zero(),
             type_traits<real>::one(),
             type_traits<real>::zero()),
        Vec3(type_traits<real>::zero(),
             type_traits<real>::zero(),
             type_traits<real>::one()),
    };
    for(size_t i = 0; i < 3; ++i)
        ret.push_back(Ray(min_corner, normals[i], max_corner[i] - min_corner[i]));
    for(size_t i = 0; i < 3; ++i)
        ret.push_back(Ray(max_corner, normals[i]*(-type_traits<real>::one()), max_corner[i] - min_corner[i]));
    //x-align,up,near; x-align,bottom,far;
    //y-align,right,near; y-align,left,far;
    //z-align,bottom,right; z-align,up,left;
    static const size_t move_indexes[][2] = {{1,2},{0,2},{0,1}};
    for(size_t i = 0; i < 3; ++i) {
        for(size_t m = 0; m < 2; ++m) {//twice
            Vec3 tmp_o = ret[i].o;
            tmp_o[move_indexes[i][m]] = max_corner[move_indexes[i][m]];
            ret.push_back(Ray(tmp_o, ret[i].n, ret[i].t));
        }
    }
}

bool AABB::get_intersection_points(const Plane &plane, std::vector<Vec3> &ret) const {
    
    std::vector<Ray> rays;
    create_edges(rays);
    
    for(auto it = rays.begin(); it != rays.end(); ++it) {
        Vec3 point;
        if(it->get_intersection_point(plane, point)) {
            ret.push_back(point);
        }
    }
    //removing duplicates
    std::sort(ret.begin(), ret.end(), [](const Vec3 &p0, const Vec3 &p1) {
        if(p0.x() == p1.x()) {
            if(p0.y() == p1.y()) {
                return p0.z() < p1.z();
            }
            return p0.y() < p1.y();
        }
        return p0.x() < p1.x();
    });
    auto end_it = std::unique(ret.begin(), ret.end());
    ret.resize(std::distance(ret.begin(), end_it));
    return true;
}

static inline bool is_ccw(const Vec3 &n, const Vec3 &v) {
    return type_traits<real>::zero() <= n.dot(v);
}

bool AABB::does_intersect(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) const {
    if(v0.equal_aproximation(v1) || v1.equal_aproximation(v2) || v2.equal_aproximation(v0)) {
        return false;
    }
    Plane plane = Triangle::create_plane(v0,v1,v2);
    //getting points on plane
    std::vector<Vec3> points;
    if(!get_intersection_points(plane, points)) {
        return false;
    }
    if(!points.size()) {//no intersection (orthogonal)
        return false;
    }
    
    //unique
    std::sort(points.begin(),points.end(), [](const Vec3 &l, const Vec3 &r) -> bool {
        if(l.x() <= r.x() && l.y() <= r.y() && l.z() < r.z()) return true;
        else return false;
    });
    auto unique_end = std::unique(points.begin(), points.end(), [](const Vec3 &l, const Vec3 &r) -> bool {
        return l.equal_aproximation(r);
    });
    points.erase(unique_end, points.end());
    
    if(points.size() > 2) {
        //checking each points
        std::vector<size_t> convex_hull_indexes;
        if(!ConvexHull2D::create(plane, points ,convex_hull_indexes)) {
            assert(0);
        }
        //assert(convex_hull_indexes.size() == points.size());
        //checking any of a triangle vertex is in all of convex_hull edges.
        const Vec3 *v_array[] = {&v0, &v1, &v2};
        for(size_t i = 0; i < 3; ++i) {
            bool ret = true;
            const Vec3 &tri_v = *v_array[i];
            for(size_t j = 0; j < convex_hull_indexes.size()-1; ++j) {
                const Vec3 ch_v0 = points[convex_hull_indexes[j]] - tri_v;
                const Vec3 ch_v1 = points[convex_hull_indexes[j+1]] - tri_v;
                if(!is_ccw(plane.n, ch_v0.cross(ch_v1))) {
                    ret = false;
                }
            }
            if(ret) return true;
        }
    }
    //checking any of a intersection point is in all of triangle edges.
    const Vec3 tri0_1 = v1 - v0;
    const Vec3 tri1_2 = v2 - v1;
    const Vec3 tri2_0 = v0 - v2;
    for(size_t i = 0; i < points.size()-1; ++i) {
        if(is_ccw(plane.n, tri0_1.cross(points[i] - v0)) &&
           is_ccw(plane.n, tri1_2.cross(points[i] - v1)) &&
           is_ccw(plane.n, tri2_0.cross(points[i] - v2)) ) {
            return true;
        }
    }
    return false;
}


bool AABB::does_intersect(const Ray &ray) const {
    //TODO
    return false;
}

