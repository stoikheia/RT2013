/**
 * 
 * @file camera.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of camera in RT2013 on 2013/08/24.
 * 
 */

#include "camera.h"

/// TODO  no tested ////

DMat<4,4> create_mat44_x_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = 1.0;
    mat.get(1,1) = std::cos(theta);
    mat.get(1,2) = -std::sin(theta);
    mat.get(2,1) = std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_y_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,2) = std::sin(theta);
    mat.get(1,1) = 1.0;
    mat.get(2,0) = -std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_z_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,1) = -std::sin(theta);
    mat.get(1,0) = std::sin(theta);
    mat.get(1,1) = std::cos(theta);
    mat.get(2,2) = 1.0;
    return mat;
}
DMat<4,4> create_mat44_zxy_rotation(real x, real y, real z) {
    return create_mat44_z_axix_rotation(z) *
    create_mat44_x_axix_rotation(x) *
    create_mat44_y_axix_rotation(y);
}

Vec3 operator*(const Vec3 &v, const DMat<4, 4> &mat) {
    return Vec3(
                v.x()*mat.get(0,0)+v.y()*mat.get(1,0)+v.z()*mat.get(2,0)+1.0*mat.get(3,0),
                v.x()*mat.get(0,1)+v.y()*mat.get(1,1)+v.z()*mat.get(2,1)+1.0*mat.get(3,1),
                v.x()*mat.get(0,2)+v.y()*mat.get(1,2)+v.z()*mat.get(2,2)+1.0*mat.get(3,2)
                );
}