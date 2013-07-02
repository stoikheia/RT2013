/**
 * 
 * @file collision_test.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of collision_test in RT2013 on 2013/06/15.
 * 
 */

#include <gtest/gtest.h>

#include "../shape/intersection.h"
#include "../shape/geom_util.h"
#include "../pair_util.hpp"

#define EXPECT_REAL_EQ(x,y) EXPECT_DOUBLE_EQ(x,y)


class IntersectionTest : public testing::Test {
protected:
    virtual void SetUp() {
        
    }
    virtual void TearDown() {
        
    }
};

TEST_F(IntersectionTest, AABBTest1)
{
    real pos_array[] = {
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 1.0};
    real tex_array[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0};
    std::vector<Vertex> vertexes;
    vertexes.push_back(Vertex(pos_array+0, tex_array+0, 0));
    vertexes.push_back(Vertex(pos_array+3, tex_array+2, 0));
    vertexes.push_back(Vertex(pos_array+6, tex_array+4, 0));
    Triangle triangle0(0,1,2);
    
    AABB aabb_tri(triangle0, vertexes);
    EXPECT_REAL_EQ(aabb_tri.min_corner.x(), 0.0);
    EXPECT_REAL_EQ(aabb_tri.min_corner.y(), 0.0);
    EXPECT_REAL_EQ(aabb_tri.min_corner.z(), 0.0);
    EXPECT_REAL_EQ(aabb_tri.max_corner.x(), 1.0);
    EXPECT_REAL_EQ(aabb_tri.max_corner.y(), 1.0);
    EXPECT_REAL_EQ(aabb_tri.max_corner.z(), 1.0);
    
    Sphere sphere0({0.0, 1.0, 0.0}, 1.0);
    AABB aabb_sphere(sphere0);
    EXPECT_REAL_EQ(aabb_sphere.min_corner.x(), -1.0);
    EXPECT_REAL_EQ(aabb_sphere.min_corner.y(), 0.0);
    EXPECT_REAL_EQ(aabb_sphere.min_corner.z(), -1.0);
    EXPECT_REAL_EQ(aabb_sphere.max_corner.x(), 1.0);
    EXPECT_REAL_EQ(aabb_sphere.max_corner.y(), 2.0);
    EXPECT_REAL_EQ(aabb_sphere.max_corner.z(), 1.0);
    
    real normald[] = {0.0,-1.0,1.0};
    Vec3 normal(normald);
    EXPECT_REAL_EQ(normal.length(), std::sqrt(2.0));
    normal.to_normal();
    EXPECT_REAL_EQ(normal.x(), 0.0);
    EXPECT_REAL_EQ(normal.y(), -1.0/std::sqrt(2.0));
    EXPECT_REAL_EQ(normal.z(), 1.0/std::sqrt(2.0));
    real ray1[] = {0.0,1.0,0.0};
    Ray ray(ray1, normal, 3.0);
    EXPECT_REAL_EQ(ray.o.x(), 0.0);
    EXPECT_REAL_EQ(ray.o.y(), 1.0);
    EXPECT_REAL_EQ(ray.o.z(), 0.0);
    EXPECT_REAL_EQ(ray.n.x(), normal.x());
    EXPECT_REAL_EQ(ray.n.y(), normal.y());
    EXPECT_REAL_EQ(ray.n.z(), normal.z());
    EXPECT_REAL_EQ(ray.t, 3.0);
    
    Ray ray2(ray);
    EXPECT_REAL_EQ(ray2.o.x(), ray.o.x());
    EXPECT_REAL_EQ(ray2.o.y(), ray.o.y());
    EXPECT_REAL_EQ(ray2.o.z(), ray.o.z());
    EXPECT_REAL_EQ(ray2.n.x(), ray.n.x());
    EXPECT_REAL_EQ(ray2.n.y(), ray.n.y());
    EXPECT_REAL_EQ(ray2.n.z(), ray.n.z());
    EXPECT_REAL_EQ(ray2.t, ray.t);
}

TEST_F(IntersectionTest, clippingTest1)//clipping test
{
    real planev[] = {0.0,0.0,-1.0};
    Plane plane(planev, 1.0);
    real normald[] = {0.0,-1.0,1.0};
    Vec3 normal(normald);
    real rayv[] = {0.0,1.0,0.0};
    Ray ray(rayv, normal.to_normal(), 3.0);
    
    Vec3 point;
    EXPECT_TRUE(ray.get_intersection_point(plane, point));
    EXPECT_REAL_EQ(point.x(), 0.0);
    EXPECT_REAL_EQ(point.y(), 0.0);
    EXPECT_REAL_EQ(point.z(), 1.0);
    
    Ray ret;
    EXPECT_TRUE(ray.get_cliped(plane, ret));
    EXPECT_REAL_EQ(ret.o.x(), 0.0);
    EXPECT_REAL_EQ(ret.o.y(), 1.0);
    EXPECT_REAL_EQ(ret.o.z(), 0.0);
    EXPECT_REAL_EQ(ret.n.x(), normal.x());
    EXPECT_REAL_EQ(ret.n.y(), normal.y());
    EXPECT_REAL_EQ(ret.n.z(), normal.z());
    EXPECT_REAL_EQ(ret.t, sqrt(2.0));
    
}

TEST_F(IntersectionTest, clippingTest2)
{
    real planev[] = {0.0,0.0,1.0};
    Plane plane(planev, 1.0);//negative direction
    real normald[] = {0.0,-1.0,1.0};
    Vec3 normal(normald);
    real rayv[] = {0.0,1.0,0.0};
    Ray ray(rayv, normal.to_normal(), 3.0);
    
    Vec3 point;
    EXPECT_FALSE(ray.get_intersection_point(plane, point));
    Ray ret;
    EXPECT_FALSE(ray.get_cliped(plane, ret));
}
TEST_F(IntersectionTest, clippingTest3)
{
    real planev[] = {0.0,0.0,-1.0};
    Plane plane(planev, 1.0);
    real normalv[] = {0.0,-1.0,1.0};
    Vec3 normal(normalv);
    real rayv[] = {0.0,1.0,0.0};
    Ray ray(rayv, normal.to_normal(), 1.0);//short
    
    Vec3 point;
    EXPECT_FALSE(ray.get_intersection_point(plane, point));
    Ray ret;
    EXPECT_FALSE(ray.get_cliped(plane, ret));
}

TEST_F(IntersectionTest, clippingTest4)//inv face clipping test
{
    real planev[] = {0.0,0.0,1.0};
    Plane plane(planev, 1.0);
    real normalv[] = {0.0,-1.0,1.0};
    Vec3 normal(normalv);
    real rayv[] = {0.0,1.0,-2.0};
    Ray ray(rayv, normal.to_normal(), 3.0);
    
    Vec3 point;
    EXPECT_TRUE(ray.get_intersection_point(plane, point));
    EXPECT_REAL_EQ(point.x(), 0.0);
    EXPECT_REAL_EQ(point.y(), 0.0);
    EXPECT_REAL_EQ(point.z(), -1.0);
    
    Ray ret;
    EXPECT_TRUE(ray.get_cliped(plane, ret));
    EXPECT_REAL_EQ(ret.o.x(), 0.0);
    EXPECT_REAL_EQ(ret.o.y(), 0.0);
    EXPECT_REAL_EQ(ret.o.z(), -1.0);
    EXPECT_REAL_EQ(ret.n.x(), normal.x());
    EXPECT_REAL_EQ(ret.n.y(), normal.y());
    EXPECT_REAL_EQ(ret.n.z(), normal.z());
    EXPECT_REAL_EQ(ret.t, 3.0 - sqrt(2.0));
    
}
TEST_F(IntersectionTest, clippingTest5)//orthogonal test
{
    real planev[] = {0.0,-1.0,0.0};
    Plane plane(planev, 1.0);
    real normalv[] = {1.0,0.0,0.0};
    Vec3 normal(normalv);
    real rayv[] = {0.0,1.0,0.0};
    Ray ray(rayv, normal.to_normal(), 1.0);
    
    Vec3 point;
    EXPECT_TRUE(ray.get_intersection_point(plane, point));
    EXPECT_REAL_EQ(point.x(), 0.0);
    EXPECT_REAL_EQ(point.y(), 1.0);
    EXPECT_REAL_EQ(point.z(), 0.0);
    Ray ret;
    EXPECT_TRUE(ray.get_cliped(plane, ret));
    EXPECT_REAL_EQ(ret.o.x(), 0.0);
    EXPECT_REAL_EQ(ret.o.y(), 1.0);
    EXPECT_REAL_EQ(ret.o.z(), 0.0);
    EXPECT_EQ(ret.n, normal);
    EXPECT_REAL_EQ(ret.t, 1.0);
}

TEST_F(IntersectionTest, aabbEdgeTest1)
{
    AABB aabb(Vec3(-1.0,-1.0,-1.0), Vec3(1.0,1.0,1.0));
    
    std::vector<Ray > ret;
    aabb.create_edges(ret);
    ASSERT_EQ(ret.size(), 12);
    
    std::vector<std::pair<Vec3, Vec3 > > validation_set;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,-1.0,-1.0), Vec3(1.0,-1.0,-1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,-1.0,-1.0), Vec3(-1.0,1.0,-1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,-1.0,-1.0), Vec3(-1.0,-1.0,1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(1.0,1.0,1.0), Vec3(-1.0,1.0,1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(1.0,1.0,1.0), Vec3(1.0,-1.0,1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(1.0,1.0,1.0), Vec3(1.0,1.0,-1.0)));
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,1.0,-1.0), Vec3(1.0,1.0,-1.0)));//x-align,up,near;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,-1.0,1.0), Vec3(1.0,-1.0,1.0)));//x-align,bottom,far;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(1.0,-1.0,-1.0), Vec3(1.0,1.0,-1.0)));//y-align,right,near;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,-1.0,1.0), Vec3(-1.0,1.0,1.0)));//y-align,left,far;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(1.0,-1.0,-1.0), Vec3(1.0,-1.0,1.0)));//z-align,bottom,right;
    validation_set.push_back(std::pair<Vec3, Vec3>(Vec3(-1.0,1.0,-1.0), Vec3(-1.0,1.0,1.0)));//z-align,up,left;
    
    const real threshold = 1.0e-3;
    for(auto it = ret.begin(); it != ret.end(); ++it) {
        std::pair<Vec3,Vec3> p = it->get_points();
        auto ret = std::find_if(validation_set.begin(), validation_set.end(),
                                [=](const std::pair<Vec3,Vec3> &vec){
            bool eq = true;
            for(size_t i = 0; i < 3; i++) {
                if(threshold < abs(vec.first.e[i] - p.first.e[i]))
                    eq = false;
            }
            if(!eq) {
                for(size_t i = 0; i < 3; i++) {
                    if(threshold < abs(vec.first.e[i] - p.second.e[i]))
                        return false;
                }
                for(size_t i = 0; i < 3; i++) {
                    if(threshold < abs(vec.second.e[i] - p.first.e[i]))
                        return false;
                }
            } else {
                for(size_t i = 0; i < 3; i++) {
                    if(threshold < abs(vec.second.e[i] - p.second.e[i]))
                        return false;
                }
            }
            return true;
        });
        ASSERT_TRUE(ret != validation_set.end());
        validation_set.erase(ret);
    }
}

TEST_F(IntersectionTest, aabbPlaneIntersectionTest1) {
    
    Vec3 normal;
    Plane plane0(Vec3(1.0,0.0,0.0), 0.0);
    Plane plane1(Vec3(0.0,1.0,0.0), 0.0);
    Plane plane2(Vec3(0.0,0.0,1.0), 0.0);
    normal.x() = 1.0; normal.y() = 0.0; normal.z() = 1.0; normal.to_normal();
    Plane plane3(normal, 1.0);
    normal.x() = 0.0; normal.y() = -1.0; normal.z() = -1.0; normal.to_normal();
    Plane plane4(normal, sqrt(2.0)-1.0e-15);
    normal.x() = 0.0; normal.y() = 1.0; normal.z() = 1.0; normal.to_normal();
    Plane plane5(normal, -sqrt(2.0)+1.0e-15);
    normal.x() = 1.0; normal.y() = 1.0; normal.z() = 1.0; normal.to_normal();
    Plane plane6(normal, 1.0);
    Plane plane7(Vec3(1.0,0.0,0.0), 1.0);
    normal = (Vec3(-1.0,0.0,1.0) - Vec3(1.0,1.0,0.0)).cross(Vec3(0.0,-1.0,-1.0) - Vec3(1.0,1.0,0.0)).to_normal();
    Plane plane8(normal, 0.0);
    
    AABB aabb(Vec3(-1.0,-1.0,-1.0), Vec3(1.0,1.0,1.0));
    
    //checking num of intersections
    std::vector<Vec3 > ret;
    ASSERT_TRUE(aabb.get_intersection_points(plane0, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    ASSERT_TRUE(aabb.get_intersection_points(plane1, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    ASSERT_TRUE(aabb.get_intersection_points(plane2, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    ASSERT_TRUE(aabb.get_intersection_points(plane3, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    
    ASSERT_TRUE(aabb.get_intersection_points(plane4, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    ASSERT_TRUE(aabb.get_intersection_points(plane5, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    
    ASSERT_TRUE(aabb.get_intersection_points(plane6, ret));
    ASSERT_EQ(ret.size(), 3);
    ret.clear();
    
    ASSERT_TRUE(aabb.get_intersection_points(plane7, ret));
    ASSERT_EQ(ret.size(), 4);
    ret.clear();
    
    ASSERT_TRUE(aabb.get_intersection_points(plane8, ret));
    ASSERT_EQ(ret.size(), 6);
    ret.clear();
}

TEST_F(IntersectionTest, aabbPlaneIntersectionTest2) {
    
    Plane plane0(Vec3(0.0,1.0,0.0), 1.0);
    
    AABB aabb(Vec3(-2.0,-2.0,-2.0), Vec3(2.0,2.0,2.0));
    std::vector<Vec3 > ret;
    ASSERT_TRUE(aabb.get_intersection_points(plane0, ret));
    ASSERT_EQ(ret.size(), 4);
    
    std::vector<Vec3> validation_set;
    validation_set.push_back(Vec3(-2.0,-1.0,-2.0));
    validation_set.push_back(Vec3(2.0,-1.0,-2.0));
    validation_set.push_back(Vec3(-2.0,-1.0,2.0));
    validation_set.push_back(Vec3(2.0,-1.0,2.0));
    
    for(auto it = ret.begin(); it != ret.end(); ++it) {
        auto pos = std::find(validation_set.begin(), validation_set.end(), *it);
        ASSERT_NE(pos, validation_set.end());
        validation_set.erase(pos);
    }
}

TEST_F(IntersectionTest, aabbTriangleIntersectionTest1) {
   
    Vertex vertexes[] = {
        Vertex(Vec3(0.5,1.0,-0.5),Vec2(0.0,0.0),0),
        Vertex(Vec3(-0.5,1.0,-0.5),Vec2(0.0,0.0),0),
        Vertex(Vec3(0.0,1.0,0.5),Vec2(0.0,0.0),0),
    };

    Triangle triangles[] = {
        Triangle(0,1,2),
    };
    
    AABB aabb(Vec3(-1.0,-1.0,-1.0), Vec3(2.0,2.0,1.5));
    
    //triangle 0
    Plane plane0 = triangles[0].create_plane(vertexes);
    
    std::vector<Vec3> points0;
    ASSERT_TRUE(aabb.get_intersection_points(plane0, points0));
    ASSERT_EQ(points0.size(), 4);
    
    std::vector<size_t> ret0;
    ASSERT_TRUE(ConvexHull2D::create(plane0, points0, ret0));
    ASSERT_EQ(ret0.size(), 4);
    
    EXPECT_REAL_EQ(points0[ret0[0]].x(), 2.0);
    EXPECT_REAL_EQ(points0[ret0[0]].y(), 1.0);
    EXPECT_REAL_EQ(points0[ret0[0]].z(), 1.5);
    
    EXPECT_REAL_EQ(points0[ret0[1]].x(), 2.0);
    EXPECT_REAL_EQ(points0[ret0[1]].y(), 1.0);
    EXPECT_REAL_EQ(points0[ret0[1]].z(), -1.0);
    
    EXPECT_REAL_EQ(points0[ret0[2]].x(), -1.0);
    EXPECT_REAL_EQ(points0[ret0[2]].y(), 1.0);
    EXPECT_REAL_EQ(points0[ret0[2]].z(), -1.0);
    
    EXPECT_REAL_EQ(points0[ret0[3]].x(), -1.0);
    EXPECT_REAL_EQ(points0[ret0[3]].y(), 1.0);
    EXPECT_REAL_EQ(points0[ret0[3]].z(), 1.5);
    
    ASSERT_TRUE(aabb.does_intersect(triangles[0], vertexes));
    
    
}


