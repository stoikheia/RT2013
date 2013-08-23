/**
 * 
 * @file geom_util.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of geom_util in RT2013 on 2013/06/15.
 * 
 */

#include "geom_util.h"

#ifdef DEBUG
#define CHECK_STRICT
#endif
//#include <iostream>//TODO

//all points are on the plane.
bool ConvexHull2D::create(const Plane &plane,
                          const std::vector<Vec3> &points,
                          std::vector<size_t> &ret) {
    assert(points.size() >= 3);
#if defined(CHECK_STRICT)
    {
        auto it = points.begin();
        const real threshold = 1.0e-10;
        for(;it != points.end();) {
            assert(std::abs((it++)->dot(plane.n) + plane.d) < threshold);
        }
    }
#endif
    std::vector<real> lens;
    for (auto it = points.begin(); it != points.end(); ++it) {
        lens.push_back(it->length());
    }
    //org_id is id of point which has maximum length.
    size_t org_id = std::distance(lens.begin(), std::max_element(lens.begin(), lens.end()));
    const Vec3 o = plane.n * -plane.d;
    const Vec3 &org_point = points[org_id];
    std::vector<std::pair<real, size_t> > theta_ids;//theta-id pair
    const Vec3 org2o_n = (o - org_point).to_normal();//normal of org to origin
    for (size_t i = 0; i < points.size(); ++i) {
        if(i != org_id) {
            Vec3 v = points[i] - org_point;
            v.to_normal();
            //this degree is right hand based
            if(is_ccw(org2o_n.cross(v), plane.n)) {
                //std::cout << points[i].x() << " "<< points[i].y() << " "<< points[i].z() << " : " << -180.0 * std::acos(org2o_n.dot(v)) / M_PI << std::endl;
                //theta_ids.push_back(std::pair<ScalarT, size_t>(-(std::acos(org2o_n.dot(v))), i));///radian
                theta_ids.push_back(std::pair<real, size_t>(-(1.0 - org2o_n.dot(v)), i));///fake degree
            } else {
                //std::cout << points[i].x() << " "<< points[i].y() << " "<< points[i].z() << " : " << 180.0 * std::acos(org2o_n.dot(v)) / M_PI << std::endl;
                //theta_ids.push_back(std::pair<ScalarT, size_t>(std::acos(org2o_n.dot(v)), i));///radian
                theta_ids.push_back(std::pair<real, size_t>(1.0 - org2o_n.dot(v), i));///fake degree
            }
        }
    }
    std::sort(theta_ids.begin(), theta_ids.end(), pair_first_greater<real,size_t>());
    
    ret.push_back(org_id);
    auto it = theta_ids.begin();
    ret.push_back(it->second); it++;
    
    for(;it != theta_ids.end();) {
        ret.push_back(it->second);
        while(!is_ccw(plane.n, get_cross(points, ret.rbegin()))) {
            ret.resize(ret.size()-2);
            ret.push_back(it->second);
            assert(ret.size() >= 3);
        }
        it++;
    }
    return true;
}

//getting normal by 3 tail poitns.
Vec3 ConvexHull2D::get_cross(const std::vector<Vec3> &points,
                             std::vector<size_t>::const_reverse_iterator it) {
    const Vec3 &v2 = points[*(it++)];
    const Vec3 &v1 = points[*(it++)];
    const Vec3 &v0 = points[*(it)];
    return (v2 - v1).cross(v0 - v1);
}

bool ConvexHull2D::is_ccw(const Vec3 &n, const Vec3 &vec) {
    return 0.0 <= n.dot(vec);
}
