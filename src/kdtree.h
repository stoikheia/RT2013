/**
 * 
 * @file kdtree.h
 * @author Reiji Tokuda.
 *
 * Interface file of kd_tree in RT2013 on 2013/06/12.
 * 
 */

#ifndef kd_tree_H
#define kd_tree_H

#include "shape/primitive.hpp"
#include "shape/geom.hpp"
#include "shape/intersection.h"

#include <vector>

class KdTree {
public:
    typedef enum {
        x = 0,
        y = 1,
        z = 2,
        non = 0xff
    } axis_t;
    
private:
    struct Element {
        size_t l_id, r_id;//child's ids
        axis_t axis;
        real min_side, max_side;
        std::vector<size_t> ids;//using leaf only
        Element():l_id(0),r_id(0) {}
    };
    
    AABB whole_aabb_;
    std::vector<Element> elements_;
    
public:
    
    KdTree(const std::vector<AABB> &aabbs);
    
    void
    get_intersection_hierarchy_ids(const Ray &ray,
                                   std::vector<std::vector<size_t> > &ret) const;
    
    void
    get_intersection_leaf_ids(const Ray &ray,
                              std::vector<size_t> &ret) const;
    
    const std::vector<size_t>
    get_ids_by_element_id(size_t elem_id) const {return elements_[elem_id].ids;};
    
private:
    void
    build_kd_tree(size_t depth,
                  size_t current_id,
                  const AABB &current_aabb,
                  const std::vector<AABB> &aabbs,
                  const std::vector<size_t> &ids);
    
    void
    build_kd_tree_impl0(size_t depth,
                        size_t current_id,
                        const AABB &current_aabb,
                        const std::vector<AABB> &aabbs,
                        const std::vector<size_t> &ids);
    
    void
    get_intersection_leafs(const Ray &ray,
                           size_t current_elem_id,
                           std::vector<size_t> &ret) const;
    
};



#endif // kd_tree_H
