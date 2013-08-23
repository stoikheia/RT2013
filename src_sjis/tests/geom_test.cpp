/**
 * 
 * @file geom_test.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of geom_tet in RT2013 on 2013/06/15.
 * 
 */

#include <gtest/gtest.h>

#include "../shape/geom.hpp"

class GeomTest : public testing::Test {
protected:
    virtual void SetUp() {
        
    }
    virtual void TearDown() {
        
    }
};

#define EXPECT_REAL_EQ(x,y) EXPECT_DOUBLE_EQ(x,y)

TEST_F(GeomTest, sphereTest1)
{
    Sphere sphere0 = {{0.0, 1.0, 2.0}, 1.0};
    EXPECT_REAL_EQ(sphere0.p.x(), 0.0);
    EXPECT_REAL_EQ(sphere0.p.y(), 1.0);
    EXPECT_REAL_EQ(sphere0.p.z(), 2.0);
    EXPECT_REAL_EQ(sphere0.r, 1.0);
}

TEST_F(GeomTest, planeTest1)
{
    
    Plane plane({0.0,0.0,-1.0}, 1.0);
    EXPECT_REAL_EQ(plane.n.x(), 0.0);
    EXPECT_REAL_EQ(plane.n.y(), 0.0);
    EXPECT_REAL_EQ(plane.n.z(), -1.0);
    EXPECT_REAL_EQ(plane.d, 1.0);
    
    Plane plane2(plane);
    EXPECT_REAL_EQ(plane2.n.x(), plane.n.x());
    EXPECT_REAL_EQ(plane2.n.y(), plane.n.y());
    EXPECT_REAL_EQ(plane2.n.z(), plane.n.z());
    EXPECT_REAL_EQ(plane2.d, 1.0);
}

TEST_F(GeomTest, triangleTest1)
{
    double pos_array[] = {
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 1.0};
    double n_array[] = {
        0.0, 0.0, 0.0,
    };
    double tex_array[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0};
    std::vector<Vertex> vertexes;
    vertexes.push_back(Vertex(pos_array+0, n_array, tex_array+0, 0));
    vertexes.push_back(Vertex(pos_array+3, n_array, tex_array+2, 0));
    vertexes.push_back(Vertex(pos_array+6, n_array, tex_array+4, 0));
    Triangle triangle0(0,1,2);
    EXPECT_EQ(triangle0.ids[0], 0);
    EXPECT_EQ(triangle0.ids[1], 1);
    EXPECT_EQ(triangle0.ids[2], 2);
    
    Plane plane0 = triangle0.create_plane(vertexes);
    EXPECT_REAL_EQ(plane0.n.x(), 1.0/std::sqrt(2.0));
    EXPECT_REAL_EQ(plane0.n.y(), -1.0/std::sqrt(2.0));
    EXPECT_REAL_EQ(plane0.n.z(), 0.0);
    EXPECT_REAL_EQ(plane0.d, 0.0);
    
}

TEST_F(GeomTest, triangleTest2)
{
    Vertex v0(Vec3(1.0,1.0,1.0), Vec3(0.0), Vec2(0.0,0.0), 0);
    Vertex v1(Vec3(-1.0,1.0,1.0), Vec3(0.0), Vec2(0.0,0.0), 0);
    Vertex v2(Vec3(-1.0,1.0,-1.0), Vec3(0.0), Vec2(0.0,0.0), 0);
    std::vector<Vertex > vertexes;
    vertexes.push_back(v0);
    vertexes.push_back(v1);
    vertexes.push_back(v2);
    
    Triangle triangle(0,1,2);
    
    Plane plane0 = triangle.create_plane(vertexes);
    EXPECT_REAL_EQ(plane0.n.x(), 0.0);
    EXPECT_REAL_EQ(plane0.n.y(), -1.0);
    EXPECT_REAL_EQ(plane0.n.z(), 0.0);
    EXPECT_REAL_EQ(plane0.d, 1.0);
}

