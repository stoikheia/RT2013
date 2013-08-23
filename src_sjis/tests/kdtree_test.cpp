/**
 * 
 * @file kdtree_test.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of kdtree_test in RT2013 on 2013/06/12.
 * 
 */

#include <gtest/gtest.h>

#include "../kdtree.h"

#define EXPECT_REAL_EQ(x,y) EXPECT_DOUBLE_EQ(x,y)

class KDTreeTest : public testing::Test {
protected:
    virtual void SetUp() {
        
    }
    virtual void TearDown() {
        
    }
};



TEST_F(KDTreeTest, sphereTest1)
{
    //20x20x20 space
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3(15.0,15.0,5.0), 5.0));
    spheres.push_back(Sphere(Vec3(1.0,19.0,19.0), 1.0));
    spheres.push_back(Sphere(Vec3(4.0,4.0,4.0), 4.0));
    spheres.push_back(Sphere(Vec3(10.0,10.0,10.0), 3.0));
    
    std::vector<AABB> aabbs;
    for(auto it = spheres.begin(); it != spheres.end(); ++it) {
        aabbs.push_back(AABB(*it));
    }
    
    KdTree kdtree(aabbs);
    
    //test0
    std::vector<std::vector<size_t> > ret0;
    Ray ray0 = Ray::create_start_end(Vec3(4.0,4.0,0.0), Vec3(4.0,4.0,20.0));
    kdtree.get_intersection_hierarchy_ids(ray0, ret0);
    ASSERT_TRUE(ret0.size() > 0);
    auto pos0 = std::find(ret0[0].begin(), ret0[0].end(), 2);
    EXPECT_NE(pos0, ret0[0].end());
    
    //test1
    std::vector<std::vector<size_t> > ret1;
    Ray ray1 = Ray::create_start_end(Vec3(20.0,0.0,0.0), Vec3(0.0,20.0,20.0));
    kdtree.get_intersection_hierarchy_ids(ray1, ret1);
    ASSERT_TRUE(ret0.size() > 0);
    std::vector<size_t> validation1;//stack
    validation1.push_back(1);
    validation1.push_back(3);
    for(size_t i = 0; i < ret1.size(); ++i) {
        for(;;) {
            auto pos1 = std::find(ret1[i].begin(), ret1[i].end(), validation1.back());
            if(pos1 != ret1[i].end()) {
                validation1.pop_back();
            } else {
                break;
            }
        }
    }
    EXPECT_TRUE(validation1.size() == 0);
    
}

#if 0

#include "obj_io.h"

struct TimeCount {
    std::chrono::high_resolution_clock::time_point time_point;
    std::string msg;
    TimeCount(const std::string &str):time_point(std::chrono::high_resolution_clock::now()),msg(str) {}
    void display_now() {
        auto duration = std::chrono::high_resolution_clock::now() - time_point;
        auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        std::cout << msg << " time : " << count/1000000 << "." << count%1000000 <<" sec" << std::endl;
    }
};

TEST_F(KDTreeTest, meshTest1)
{
    std::vector<Triangle> triangles;
    std::vector<Vertex> vertexes;
    
    {
        TimeCount tc("load_obj_vnf");
        ASSERT_TRUE(load_obj_vnf("./buddha3.obj", triangles, vertexes));
        tc.display_now();
    }
    
    std::vector<AABB> aabbs;
    {
        TimeCount tc("aabb creation");
        for(auto it = triangles.begin(); it != triangles.end(); ++it) {
            aabbs.push_back(AABB(*it, vertexes));
        }
        tc.display_now();
    }
    {
        TimeCount tc("kdtree creation");
        KdTree kdtree(aabbs);
        tc.display_now();
        {
            Ray ray(Vec3(-5.0), Vec3(1.0,1.0,1.0).to_normal());
            std::vector<std::vector<size_t> > ret;
            TimeCount tc("hierarchy search1");
            kdtree.get_intersection_hierarchy_ids(ray, ret);
            tc.display_now();
            size_t num_aabb = 0;
            for(size_t i = 0; i < ret.size(); ++i) {
                std::cout << "ret sub size : "<< i << " : " << ret[i].size() << std::endl;
                num_aabb += ret[i].size();
            }
            std::cout << "num kdtree area : " << ret.size() << std::endl;
            std::cout << "ret size : " << num_aabb << std::endl;
        }
        {
            Ray ray(Vec3(-5.0), Vec3(1.0,1.0,1.0).to_normal());
            std::vector<size_t> ret;
            TimeCount tc("leaf search1");
            kdtree.get_intersection_leaf_ids(ray, ret);
            tc.display_now();
            for(size_t i = 0; i < ret.size(); ++i) {
                std::cout << "ret id : "<< i << " : " << ret[i] << std::endl;
            }
            std::cout << "num kdtree area : " << ret.size() << std::endl;
            
            //has intersection?
            real length = type_traits<real>::inf();
            Vec3 point;
            for(size_t i = 0; i < ret.size(); ++i) {
                //std::cout << "ret id : "<< i << " : " << ret[i] << std::endl;
                const std::vector<size_t> &ids = kdtree.get_ids_by_element_id(ret[i]);
                for(size_t j = 0; j < ids.size(); ++j) {
                    Vec3 ret;
                    real len, direction;
                    real min_length = length;//for checking min:i.block <= min:(i+1).block
                    if(ray.get_intersection_point(triangles[ids[j]], vertexes, ret, len, direction)) {
                        std::cout << "intersection detected : elemnt" << i << "[" << j << "] length = " << len << (direction<=0?" positive":" negative") << std::endl;
                        if(min_length != type_traits<real>::inf()) {
                            ASSERT_TRUE(min_length <= len);//check
                        }
                        if(len < length) {
                            std::cout << "new nearest point" << std::endl;
                            point = ret;
                            length = len;
                        }
                    }
                }
            }
            ASSERT_TRUE(length != type_traits<real>::inf());
        }
    }
    
#if 0
    {
        TimeCount tc("kdtree creation");
        KdTree kdtree(aabbs, triangles, vertexes);
        tc.display_now();
        {
            Ray ray(Vec3(-5.0), Vec3(1.0,1.0,1.0).to_normal());
            std::vector<std::vector<size_t> > ret;
            TimeCount tc("hierarchy search1");
            kdtree.get_intersection_hierarchy_ids(ray, ret);
            tc.display_now();
            size_t num_aabb = 0;
            for(size_t i = 0; i < ret.size(); ++i) {
                std::cout << "ret sub size : "<< i << " : " << ret[i].size() << std::endl;
                num_aabb += ret[i].size();
            }
            std::cout << "num kdtree area : " << ret.size() << std::endl;
            std::cout << "ret size : " << num_aabb << std::endl;
        }
        {
            Ray ray(Vec3(-5.0), Vec3(1.0,1.0,1.0).to_normal());
            std::vector<size_t> ret;
            TimeCount tc("leaf search1");
            kdtree.get_intersection_leaf_ids(ray, ret);
            tc.display_now();
            for(size_t i = 0; i < ret.size(); ++i) {
                std::cout << "ret id : "<< i << " : " << ret[i] << std::endl;
            }
            std::cout << "num kdtree area : " << ret.size() << std::endl;
            
            //has intersection?
            real length = type_traits<real>::inf();
            Vec3 point;
            for(size_t i = 0; i < ret.size(); ++i) {
                //std::cout << "ret id : "<< i << " : " << ret[i] << std::endl;
                const std::vector<size_t> &ids = kdtree.get_ids_by_element_id(ret[i]);
                for(size_t j = 0; j < ids.size(); ++j) {
                    Vec3 ret;
                    real len, direction;
                    real min_length = length;//for checking min:i.block <= min:(i+1).block
                    if(ray.get_intersection_point(triangles[ids[j]], vertexes, ret, len, direction)) {
                        std::cout << "intersection detected : elemnt" << i << "[" << j << "] length = " << len << (direction<=0?" positive":" negative") << std::endl;
                        if(min_length != type_traits<real>::inf()) {
                            ASSERT_TRUE(min_length <= len);//check
                        }
                        if(len < length) {
                            std::cout << "new nearest point" << std::endl;
                            point = ret;
                            length = len;
                        }
                    }
                }
            }
            ASSERT_TRUE(length != type_traits<real>::inf());
        }
    }
#endif
}

#endif