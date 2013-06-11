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
    Vec3<double> v0(1.0f);
    EXPECT_EQ(v0.x(), 1.0f);
    EXPECT_EQ(v0.y(), 1.0f);
    EXPECT_EQ(v0.z(), 1.0f);
    
    Vec3<double> v1(1.0f,2.0f,3.0f);
    EXPECT_EQ(v1.x(), 1.0f);
    EXPECT_EQ(v1.y(), 2.0f);
    EXPECT_EQ(v1.z(), 3.0f);
    
    Mat<double, 3, 4> mat0 = {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f};
    EXPECT_EQ(mat0.get(0,0), 0.0f);
    EXPECT_EQ(mat0.get(0,1), 0.1f);
    EXPECT_EQ(mat0.get(0,2), 0.2f);
    EXPECT_EQ(mat0.get(0,3), 0.3f);
    EXPECT_EQ(mat0.get(1,0), 1.0f);
    EXPECT_EQ(mat0.get(1,1), 1.1f);
    EXPECT_EQ(mat0.get(1,2), 1.2f);
    EXPECT_EQ(mat0.get(1,3), 1.3f);
    EXPECT_EQ(mat0.get(2,0), 2.0f);
    EXPECT_EQ(mat0.get(2,1), 2.1f);
    EXPECT_EQ(mat0.get(2,2), 2.2f);
    EXPECT_EQ(mat0.get(2,3), 2.3f);
    
    //move
    double* p1 = v1.p->data();
    Vec3<double> v3(std::move(v1));
    ASSERT_EQ(p1, v3.p->data());
}


TEST_F(PrimitiveTest, primitiveTest2)
{
}