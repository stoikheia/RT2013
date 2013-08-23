/**
 * 
 * @file kdtree.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of kd_tree in RT2013 on 2013/06/12.
 * 
 */

#include "kdtree.h"

#include <algorithm>

/** macros for logging. */
//#define OUTPUT_LOGS
#ifdef OUTPUT_LOGS
#define _LOG(x) x
#include <iostream>
#else
#define _LOG(x)
#endif


const size_t kMIN_DEPTH = 6;
const size_t kNUM_LEAF_EXPECTED_IDS = 5;
const real kZERO_DIV_THRESHOLD = 0.5;

inline size_t
get_max_depth(size_t size) {
    size_t depth = 0, i = 1, ii = 1;
    while(depth < kMIN_DEPTH) {
        i *= 2;
        ii += i;
        ++depth;
    }
    while((size / i) > kNUM_LEAF_EXPECTED_IDS) {
        i *= 2;
        ii += i;
        ++depth;
    }
    _LOG(std::cout << "kd tree max depth : " << depth << " num leaf : " << i << " num node : " << ii << std::endl);
    return depth;
}

inline KdTree::axis_t
create_axis(const AABB &aabb) {
    real x,y,z;
    x = aabb.max_corner.x() - aabb.min_corner.x();
    y = aabb.max_corner.y() - aabb.min_corner.y();
    z = aabb.max_corner.z() - aabb.min_corner.z();
    if(x > y && x > z) {
        return KdTree::axis_t::x;
    } else if(y > z) {
        return KdTree::axis_t::y;
    } else {
        return KdTree::axis_t::z;
    }
}

inline real
get_div_point(const std::vector<size_t> &sorted_ids, const std::vector<AABB> &aabbs, KdTree::axis_t axis) {
    
    assert(sorted_ids.size() >= 2);
    size_t id0 = sorted_ids[sorted_ids.size()/2-1];
    size_t id1 = sorted_ids[sorted_ids.size()/2];
    return (aabbs[id0].min_corner[axis] + aabbs[id1].min_corner[axis]) / 2.0;
}




KdTree::KdTree(const std::vector<AABB> &aabbs) {
    
    //creating id array
    _LOG(std::cout << "Kdtree - creating ids" << "\r");
    std::vector<size_t> ids;
    for(size_t i = 0; i < aabbs.size(); ++i) {
        ids.push_back(i);
    }
    
    //creating all contains aabb
    _LOG(std::cout << "Kdtree - creating whole aabb" << "\r");
	//VC
    std::vector<AABB>::const_iterator it = aabbs.begin();
    AABB whole_aabb(*it);
    while(++it != aabbs.end()) {
        whole_aabb.merge(*it);
    }
    whole_aabb_ = whole_aabb;
    
    elements_.push_back(Element());//root
    elements_[0].axis = x;
    elements_[0].min_side = whole_aabb_.min_corner[x];
    elements_[0].max_side = whole_aabb_.max_corner[x];
    build_kd_tree(get_max_depth(aabbs.size()), 0, whole_aabb_, aabbs, ids);
};

//aabb ids must be identical to triangle ids
KdTree::KdTree(const std::vector<AABB> &aabbs,//triangles aabb
               const std::vector<Triangle> &triangles,
               const std::vector<Vertex> &vertexes) {
    
    //creating id array
    _LOG(std::cout << "Kdtree - creating ids" << "\r");
    std::vector<size_t> ids;
    for(size_t i = 0; i < aabbs.size(); ++i) {
        ids.push_back(i);
    }
    
    //creating all contains aabb
    _LOG(std::cout << "Kdtree - creating whole aabb" << "\r");
	//VC
    std::vector<AABB>::const_iterator it = aabbs.begin();
    AABB whole_aabb(*it);
    while(++it != aabbs.end()) {
        whole_aabb.merge(*it);
    }
    whole_aabb_ = whole_aabb;
    
    elements_.push_back(Element());//root
    elements_[0].axis = x;
    elements_[0].min_side = whole_aabb_.min_corner[x];
    elements_[0].max_side = whole_aabb_.max_corner[x];
    build_kd_tree(get_max_depth(aabbs.size()), 0, whole_aabb_, aabbs, triangles, vertexes, ids);
    
}

//balanced with locally zero area (with triangle intersection check)
void
KdTree::build_kd_tree(size_t depth,
                      size_t current_id,//element_id
                      const AABB &current_aabb,
                      const std::vector<AABB> &aabbs,//all aabbs
                      const std::vector<Triangle> &triangles,//all triangles
                      const std::vector<Vertex> &vertexes,//all vertexes
                      const std::vector<size_t> &ids) {//aabb/triangle ids
    
    if(depth == 0 || ids.size() < 2) {
        elements_[current_id].ids.assign(ids.begin(), ids.end());
#if 0
        std::cout << "leaf - depth : " << depth << " node id : " << current_id << " num : " << ids.size() << "------"<< std::endl;
        std::cout << "aabb min : " << current_aabb.min_corner.x() << "," << current_aabb.min_corner.y() << "," << current_aabb.min_corner.z() << std::endl;
        std::cout << "aabb max : " << current_aabb.max_corner.x() << "," << current_aabb.max_corner.y() << "," << current_aabb.max_corner.z() << std::endl;
#endif
        return;
    }
    
    axis_t axis = create_axis(current_aabb);
    
    auto minmax_its = std::minmax_element(ids.begin(), ids.end(), [axis, &aabbs](size_t l, size_t r) -> bool {
        return aabbs[l].min_corner[axis] < aabbs[r].min_corner[axis];
    });
    
    real min_val, max_val;
    min_val = std::max(aabbs[*minmax_its.first].min_corner[axis], current_aabb.min_corner[axis]);
    max_val = std::min(aabbs[*minmax_its.second].max_corner[axis], current_aabb.max_corner[axis]);
    
    real div_point;
    bool l_is_zero = false, r_is_zero = false;
    
    if((max_val - current_aabb.min_corner[axis]) / (current_aabb.max_corner[axis] - current_aabb.min_corner[axis]) <= kZERO_DIV_THRESHOLD) {
        div_point = max_val;
        r_is_zero = true;
    } else if ((current_aabb.max_corner[axis] - min_val) / (current_aabb.max_corner[axis] - current_aabb.min_corner[axis]) <= kZERO_DIV_THRESHOLD) {
        div_point = min_val;
        l_is_zero = true;
    } else {
        std::vector<size_t> sorted_ids(ids);
        std::sort(sorted_ids.begin(),
                  sorted_ids.end(),
                  [axis, &aabbs](size_t l, size_t r) -> bool {
                      return aabbs[l].min_corner[axis] < aabbs[r].min_corner[axis];
                  });
        
        div_point = get_div_point(sorted_ids, aabbs, axis);
        
        if((div_point <= current_aabb.min_corner[axis]) ||
           (div_point >= current_aabb.max_corner[axis])) {
            div_point = (current_aabb.min_corner[axis] + current_aabb.max_corner[axis]) / 2.0;
        }
    }
    
    
    //divide aabb
    AABB l_aabb(current_aabb), r_aabb(current_aabb);
    l_aabb.max_corner[axis] = div_point;
    r_aabb.min_corner[axis] = div_point;
    
    //next elements
    size_t l_id, r_id;
    l_id = elements_.size();
    elements_.push_back(Element());
    r_id = elements_.size();
    elements_.push_back(Element());
    
    
    elements_[current_id].l_id = l_id;
    elements_[current_id].r_id = r_id;
    
    elements_[l_id].axis = axis;
    elements_[l_id].min_side = current_aabb.min_corner[axis];
    elements_[l_id].max_side = div_point;
    
    elements_[r_id].axis = axis;
    elements_[r_id].min_side = div_point;
    elements_[r_id].max_side = current_aabb.max_corner[axis];
    
    if(l_is_zero) {
        build_kd_tree(depth, l_id, l_aabb, aabbs, std::vector<size_t>());
        build_kd_tree(depth, r_id, r_aabb, aabbs, ids);
    } else if(r_is_zero) {
        build_kd_tree(depth, l_id, l_aabb, aabbs, ids);
        build_kd_tree(depth, r_id, r_aabb, aabbs, std::vector<size_t>());
    } else {
        //divide ids
        std::vector<size_t> l_ids, r_ids;
        for (auto it = ids.begin(); it != ids.end(); ++it) {
            if(aabbs[*it].min_corner[axis] <= div_point &&
               l_aabb.does_intersect(triangles[*it], vertexes)) {
                l_ids.push_back(*it);
            }
            if(aabbs[*it].max_corner[axis] >= div_point &&
               r_aabb.does_intersect(triangles[*it], vertexes)) {
                r_ids.push_back(*it);
            }
        }
        
        build_kd_tree(depth-1, l_id, l_aabb, aabbs, triangles, vertexes, l_ids);
        build_kd_tree(depth-1, r_id, r_aabb, aabbs, triangles, vertexes, r_ids);
    }
}

//balanced with locally zero area
void
KdTree::build_kd_tree(size_t depth,
                      size_t current_id,//element_id
                      const AABB &current_aabb,
                      const std::vector<AABB> &aabbs,//all aabbs
                      const std::vector<size_t> &ids) {//aabb ids
    
    if(depth == 0 || ids.size() < 2) {
        elements_[current_id].ids.assign(ids.begin(), ids.end());
#if 0
        std::cout << "leaf - depth : " << depth << " node id : " << current_id << " num : " << ids.size() << "------"<< std::endl;
        std::cout << "aabb min : " << current_aabb.min_corner.x() << "," << current_aabb.min_corner.y() << "," << current_aabb.min_corner.z() << std::endl;
        std::cout << "aabb max : " << current_aabb.max_corner.x() << "," << current_aabb.max_corner.y() << "," << current_aabb.max_corner.z() << std::endl;
#endif
        return;
    }
    
    axis_t axis = create_axis(current_aabb);
    
    auto minmax_its = std::minmax_element(ids.begin(), ids.end(), [axis, &aabbs](size_t l, size_t r) -> bool {
        return aabbs[l].min_corner[axis] < aabbs[r].min_corner[axis];
    });
    
    real min_val, max_val;
    min_val = std::max(aabbs[*minmax_its.first].min_corner[axis], current_aabb.min_corner[axis]);
    max_val = std::min(aabbs[*minmax_its.second].max_corner[axis], current_aabb.max_corner[axis]);
    
    real div_point;
    bool l_is_zero = false, r_is_zero = false;
    
    if((max_val - current_aabb.min_corner[axis]) / (current_aabb.max_corner[axis] - current_aabb.min_corner[axis]) <= kZERO_DIV_THRESHOLD) {
        div_point = max_val;
        r_is_zero = true;
    } else if ((current_aabb.max_corner[axis] - min_val) / (current_aabb.max_corner[axis] - current_aabb.min_corner[axis]) <= kZERO_DIV_THRESHOLD) {
        div_point = min_val;
        l_is_zero = true;
    } else {
        std::vector<size_t> sorted_ids(ids);
        std::sort(sorted_ids.begin(),
                  sorted_ids.end(),
                  [axis, &aabbs](size_t l, size_t r) -> bool {
                      return aabbs[l].min_corner[axis] < aabbs[r].min_corner[axis];
                  });
        
        div_point = get_div_point(sorted_ids, aabbs, axis);
        
        if((div_point <= current_aabb.min_corner[axis]) ||
           (div_point >= current_aabb.max_corner[axis])) {
            div_point = (current_aabb.min_corner[axis] + current_aabb.max_corner[axis]) / 2.0;
        }
    }
    
    
    //divide aabb
    AABB l_aabb(current_aabb), r_aabb(current_aabb);
    l_aabb.max_corner[axis] = div_point;
    r_aabb.min_corner[axis] = div_point;
    
    //next elements
    size_t l_id, r_id;
    l_id = elements_.size();
    elements_.push_back(Element());
    r_id = elements_.size();
    elements_.push_back(Element());
    
    
    elements_[current_id].l_id = l_id;
    elements_[current_id].r_id = r_id;
    
    elements_[l_id].axis = axis;
    elements_[l_id].min_side = current_aabb.min_corner[axis];
    elements_[l_id].max_side = div_point;
    
    elements_[r_id].axis = axis;
    elements_[r_id].min_side = div_point;
    elements_[r_id].max_side = current_aabb.max_corner[axis];
    
    if(l_is_zero) {
        build_kd_tree(depth, l_id, l_aabb, aabbs, std::vector<size_t>());
        build_kd_tree(depth, r_id, r_aabb, aabbs, ids);
    } else if(r_is_zero) {
        build_kd_tree(depth, l_id, l_aabb, aabbs, ids);
        build_kd_tree(depth, r_id, r_aabb, aabbs, std::vector<size_t>());
    } else {
        //divide ids
        std::vector<size_t> l_ids, r_ids;
        for (auto it = ids.begin(); it != ids.end(); ++it) {
            if(aabbs[*it].min_corner[axis] <= div_point) {
                l_ids.push_back(*it);
            }
            if(aabbs[*it].max_corner[axis] >= div_point) {
                r_ids.push_back(*it);
            }
        }
        
        build_kd_tree(depth-1, l_id, l_aabb, aabbs, l_ids);
        build_kd_tree(depth-1, r_id, r_aabb, aabbs, r_ids);
    }
}

//simple balanced
void
KdTree::build_kd_tree_impl0(size_t depth,
                            size_t current_id,//element_id
                            const AABB &current_aabb,
                            const std::vector<AABB> &aabbs,//all aabbs
                            const std::vector<size_t> &ids) {//aabb ids
    
    if(depth == 0 || ids.size() < 2) {
        elements_[current_id].ids.assign(ids.begin(), ids.end());
#if 0
        std::cout << "leaf - depth : " << depth << " node id : " << current_id << " num : " << ids.size() << "------"<< std::endl;
        std::cout << "aabb min : " << current_aabb.min_corner.x() << "," << current_aabb.min_corner.y() << "," << current_aabb.min_corner.z() << std::endl;
        std::cout << "aabb max : " << current_aabb.max_corner.x() << "," << current_aabb.max_corner.y() << "," << current_aabb.max_corner.z() << std::endl;
#endif
        return;
    }
    
    axis_t axis = create_axis(current_aabb);
    
    std::vector<size_t> sorted_ids(ids);
    std::sort(sorted_ids.begin(),
              sorted_ids.end(),
              [axis, &aabbs](size_t l, size_t r) -> bool {
                  return aabbs[l].min_corner[axis] < aabbs[r].min_corner[axis];
              });
    
    real div_point = get_div_point(sorted_ids, aabbs, axis);
    
    if(div_point <= current_aabb.min_corner[axis]) {
        div_point = current_aabb.min_corner[axis] + current_aabb.max_corner[axis] / 2.0;
    }
    
    //next elements
    size_t l_id, r_id;
    l_id = elements_.size();
    elements_.push_back(Element());
    r_id = elements_.size();
    elements_.push_back(Element());
    
    //divide aabb
    AABB l_aabb(current_aabb), r_aabb(current_aabb);
    l_aabb.max_corner[axis] = div_point;
    r_aabb.min_corner[axis] = div_point;
    
    //divide ids
    std::vector<size_t> l_ids, r_ids;
    for (auto it = ids.begin(); it != ids.end(); ++it) {
        if(aabbs[*it].min_corner[axis] <= div_point) {
            l_ids.push_back(*it);
        }
        if(aabbs[*it].max_corner[axis] >= div_point) {
            r_ids.push_back(*it);
        }
    }
    
    elements_[current_id].l_id = l_id;
    elements_[current_id].r_id = r_id;
    
    elements_[l_id].axis = axis;
    elements_[l_id].min_side = current_aabb.min_corner[axis];
    elements_[l_id].max_side = div_point;
    
    elements_[r_id].axis = axis;
    elements_[r_id].min_side = div_point;
    elements_[r_id].max_side = current_aabb.max_corner[axis];
    
    build_kd_tree_impl0(depth-1, l_id, l_aabb, aabbs, l_ids);
    build_kd_tree_impl0(depth-1, r_id, r_aabb, aabbs, r_ids);
}

Plane
create_plane(bool positive, KdTree::axis_t axis, const real &d) {
    const Vec3 normals[] = {
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
    return Plane(normals[axis] * (positive?1.0:-1.0), positive?-d:d);
}

//for get hit *leaf ids* recursively.
//ids are sorted by distance from ray origin.
void
KdTree::get_intersection_leafs(const Ray &ray, size_t current_elem_id, std::vector<size_t> &ret) const {
    
    const Element &current = elements_[current_elem_id];
    
    //checking intersection and clipping ray
    Plane min_plane = create_plane(true, current.axis, current.min_side);
    Plane max_plane = create_plane(false, current.axis, current.max_side);
    
    Ray cliped_ray0;
    if(!ray.get_cliped(min_plane, cliped_ray0)) {
        return;
    }
    Ray cliped_ray1;
    if(!cliped_ray0.get_cliped(max_plane, cliped_ray1)) {
        return;
    }
    
    //leaf
    if(elements_[current_elem_id].l_id == 0) {
        assert(!elements_[current_elem_id].r_id);
        ret.push_back(current_elem_id);
        return;
    }
    
    const Element &l = elements_[current.l_id];
    const Element &r = elements_[current.r_id];
    assert(l.max_side == r.min_side);
    if(cliped_ray1.o[l.axis] <= l.max_side) {
        get_intersection_leafs(cliped_ray1, current.l_id, ret);//l first
        get_intersection_leafs(cliped_ray1, current.r_id, ret);
    } else {
        get_intersection_leafs(cliped_ray1, current.r_id, ret);//r first
        get_intersection_leafs(cliped_ray1, current.l_id, ret);
    }
}

//for getting *leaf ids* which is sorted by distance from ray origin.
void
KdTree::get_intersection_leaf_ids(const Ray &ray, std::vector<size_t> &ret) const {
    get_intersection_leafs(ray, 0, ret);
}

//for getting *ids in leafs* which is sorted by distance from ray origin each elements.
//removed duplicated ids.
void
KdTree::get_intersection_hierarchy_ids(const Ray &ray, std::vector<std::vector<size_t> > &ret) const {
    
    std::vector<size_t> leaf_ids;
    get_intersection_leafs(ray, 0, leaf_ids);
    
    ret.resize(leaf_ids.size());
    std::vector<size_t> duplicates;
    for(size_t i = 0; i < leaf_ids.size(); ++i) {
        const Element &e = elements_[leaf_ids[i]];
        std::vector<size_t> &current_ids = ret[i];
        for(auto it = e.ids.begin(); it != e.ids.end(); ++it) {
            if(!std::binary_search(duplicates.begin(), duplicates.end(), *it)) {
                current_ids.push_back(*it);
            }
        }
        duplicates.insert(duplicates.end(), current_ids.begin(), current_ids.end());
        std::sort(duplicates.begin(), duplicates.end());
        auto pos = std::unique(duplicates.begin(), duplicates.end());
        duplicates.resize(std::distance(duplicates.begin(), pos));
    }
}


#ifdef _LOG
#undef _LOG
#endif



