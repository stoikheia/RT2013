/**
 * 
 * @file File.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of File in RT2013 on 2013/06/12.
 * 
 */

#include <gtest/gtest.h>

#include "../shape/primitive.hpp"

class PrimitiveTest : public testing::Test {
protected:
    virtual void SetUp() {
        
    }
    virtual void TearDown() {
        
    }
};

TEST_F(PrimitiveTest, primitiveTest1)
{
    ///// std::array is movable only if its contained objects are movable.
    std::array<double, 4> move_test = {0.0, 1.0, 2.0, 3.0};
    double *move_test_p = move_test.data();
    std::array<double, 4> move_test_receive(std::move(move_test));
    EXPECT_NE(move_test_p, move_test_receive.data());
    EXPECT_DOUBLE_EQ(move_test_receive[0], 0.0);
    EXPECT_DOUBLE_EQ(move_test_receive[2], 2.0);
    /////
    
    Vec3<double> v0(1.0f);
    EXPECT_DOUBLE_EQ(v0.x(), 1.0f);
    EXPECT_DOUBLE_EQ(v0.y(), 1.0f);
    EXPECT_DOUBLE_EQ(v0.z(), 1.0f);
    
    Vec3<double> v1(1.0f,2.0f,3.0f);
    EXPECT_DOUBLE_EQ(v1.x(), 1.0f);
    EXPECT_DOUBLE_EQ(v1.y(), 2.0f);
    EXPECT_DOUBLE_EQ(v1.z(), 3.0f);
    
    Mat<double, 3, 4> mat0 = {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f};
    EXPECT_DOUBLE_EQ(mat0.get(0,0), 0.0f);
    EXPECT_DOUBLE_EQ(mat0.get(0,1), 0.1f);
    EXPECT_DOUBLE_EQ(mat0.get(0,2), 0.2f);
    EXPECT_DOUBLE_EQ(mat0.get(0,3), 0.3f);
    EXPECT_DOUBLE_EQ(mat0.get(1,0), 1.0f);
    EXPECT_DOUBLE_EQ(mat0.get(1,1), 1.1f);
    EXPECT_DOUBLE_EQ(mat0.get(1,2), 1.2f);
    EXPECT_DOUBLE_EQ(mat0.get(1,3), 1.3f);
    EXPECT_DOUBLE_EQ(mat0.get(2,0), 2.0f);
    EXPECT_DOUBLE_EQ(mat0.get(2,1), 2.1f);
    EXPECT_DOUBLE_EQ(mat0.get(2,2), 2.2f);
    EXPECT_DOUBLE_EQ(mat0.get(2,3), 2.3f);
    
    //move
    double* p1 = v1.p->data();
    Vec3<double> v3(std::move(v1));
    ASSERT_EQ(p1, v3.p->data());
    
    std::vector<Vec3<double> > tri_array = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 0.0},
        {1.0, 1.0, 1.0}};
    Triangle triangle0(0,1,2);
    AABB<double> aabb_tri(triangle0, tri_array);
    EXPECT_DOUBLE_EQ(aabb_tri.min_corner.x(), 0.0);
    EXPECT_DOUBLE_EQ(aabb_tri.min_corner.y(), 0.0);
    EXPECT_DOUBLE_EQ(aabb_tri.min_corner.z(), 0.0);
    EXPECT_DOUBLE_EQ(aabb_tri.max_corner.x(), 1.0);
    EXPECT_DOUBLE_EQ(aabb_tri.max_corner.y(), 1.0);
    EXPECT_DOUBLE_EQ(aabb_tri.max_corner.z(), 1.0);
    
    Sphere<double> sphere0({0.0, 1.0, 0.0}, 1.0);
    AABB<double> aabb_sphere(sphere0);
    EXPECT_DOUBLE_EQ(aabb_sphere.min_corner.x(), -1.0);
    EXPECT_DOUBLE_EQ(aabb_sphere.min_corner.y(), 0.0);
    EXPECT_DOUBLE_EQ(aabb_sphere.min_corner.z(), -1.0);
    EXPECT_DOUBLE_EQ(aabb_sphere.max_corner.x(), 1.0);
    EXPECT_DOUBLE_EQ(aabb_sphere.max_corner.y(), 2.0);
    EXPECT_DOUBLE_EQ(aabb_sphere.max_corner.z(), 1.0);
    
    
}


TEST_F(PrimitiveTest, primitiveTest2)
{
}