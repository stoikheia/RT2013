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

#define EXPECT_REAL_EQ(x,y) EXPECT_DOUBLE_EQ(x,y)

TEST_F(PrimitiveTest, primitiveTest1)
{
    ///// std::array is movable only if its contained objects are movable.
//    std::array<double, 4> move_test = {0.0, 1.0, 2.0, 3.0};
//    double *move_test_p = move_test.data();
//    std::array<double, 4> move_test_receive(std::move(move_test));
//    EXPECT_NE(move_test_p, move_test_receive.data());
//    EXPECT_REAL_EQ(move_test_receive[0], 0.0);
//    EXPECT_REAL_EQ(move_test_receive[2], 2.0);
    /////
    
    Vec3 v0(1.0);
    EXPECT_REAL_EQ(v0.x(), 1.0);
    EXPECT_REAL_EQ(v0.y(), 1.0);
    EXPECT_REAL_EQ(v0.z(), 1.0);
    
    Vec3 v1(1.0,2.0,3.0);
    EXPECT_REAL_EQ(v1.x(), 1.0);
    EXPECT_REAL_EQ(v1.y(), 2.0);
    EXPECT_REAL_EQ(v1.z(), 3.0);
    
    double p0[] = {
                  0.0, 0.1, 0.2, 0.3,
                  1.0, 1.1, 1.2, 1.3,
                  2.0, 2.1, 2.2, 2.3};
    DMat<3, 4> mat0(p0);
    EXPECT_REAL_EQ(mat0.get(0,0), 0.0);
    EXPECT_REAL_EQ(mat0.get(0,1), 0.1);
    EXPECT_REAL_EQ(mat0.get(0,2), 0.2);
    EXPECT_REAL_EQ(mat0.get(0,3), 0.3);
    EXPECT_REAL_EQ(mat0.get(1,0), 1.0);
    EXPECT_REAL_EQ(mat0.get(1,1), 1.1);
    EXPECT_REAL_EQ(mat0.get(1,2), 1.2);
    EXPECT_REAL_EQ(mat0.get(1,3), 1.3);
    EXPECT_REAL_EQ(mat0.get(2,0), 2.0);
    EXPECT_REAL_EQ(mat0.get(2,1), 2.1);
    EXPECT_REAL_EQ(mat0.get(2,2), 2.2);
    EXPECT_REAL_EQ(mat0.get(2,3), 2.3);
    
//    double *p = mat0.e;
//    DMat<3, 4> mat1(std::move(mat0));
//    ASSERT_EQ(p, mat1.e);
//    ASSERT_TRUE(mat0.e == NULL);
    
}
TEST_F(PrimitiveTest, primitiveTest2) {
    Vec3 v0(1.0);
    Vec3 v1(1.0,1.0,1.0);
    ASSERT_TRUE(v0 == v1);
}

TEST_F(PrimitiveTest, matrixTest1) {
    double p0[] = {
        0.0, 0.1, 0.2, 0.3,
        1.0, 1.1, 1.2, 1.3,
        2.0, 2.1, 2.2, 2.3};
    DMat<3, 4> mat0(p0);
    double p1[] = {
        0.0, 0.1, 0.2,};
    DMat<1, 3> mat1(p1);
    
    DMat<1,4> ret = mat1 * mat0;
    EXPECT_REAL_EQ(ret.get(0,0), 0.0*0.0 + 0.1*1.0 + 0.2*2.0);
    EXPECT_REAL_EQ(ret.get(0,1), 0.0*0.1 + 0.1*1.1 + 0.2*2.1);
    EXPECT_REAL_EQ(ret.get(0,2), 0.0*0.2 + 0.1*1.2 + 0.2*2.2);
    EXPECT_REAL_EQ(ret.get(0,3), 0.0*0.3 + 0.1*1.3 + 0.2*2.3);
}


