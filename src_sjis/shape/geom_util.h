/**
 * 
 * @file geom_util.h
 * @author Reiji Tokuda.
 *
 * Interface file of geom_util in RT2013 on 2013/06/15.
 * 
 */

#ifndef geom_util_H
#define geom_util_H

#include "geom.hpp"
#include "../pair_util.hpp"


struct ConvexHull2D {

    //all points are on the plane.
    static bool create(const Plane &plane,
                       const std::vector<Vec3> &points,
                       std::vector<size_t> &ret);
    
private:
    //getting normal by 3 tail poitns.
    static Vec3 get_cross(const std::vector<Vec3> &points,
                          std::vector<size_t>::const_reverse_iterator it);
    
    static bool is_ccw(const Vec3 &n, const Vec3 &vec);
};


#endif // geom_util_H
