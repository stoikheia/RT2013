/**
 * 
 * @file camera.h
 * @author Reiji Tokuda.
 *
 * Interface file of camera in RT2013 on 2013/08/24.
 * 
 */

#ifndef camera_H
#define camera_H

#include "shape/primitive.hpp"
#include "shape/intersection.h"
#include "screen_buffer.h"
#include <cmath>

inline real to_rad(real degree) {
    return degree * M_PI / 180.0;
}

Vec3 operator*(const Vec3 &v, const DMat<4, 4> &mat);

struct Camera {
    real fov;//horizontal fov
    real aspect;
    Vec3 pos;
    DMat<4,4> mat;
    size_t samples;
    size_t sub_samples;
    
private:
    real v_fov;
    real plane_width;
    real plane_height;
    real plane_origin_h;
    real plane_origin_v;
public:
    
    void setup() {
        v_fov = fov / aspect;
        plane_width = 2.0 * (sin(fov/2.0)/cos(fov/2.0));
        plane_height = 2.0 * (sin(v_fov/2.0)/cos(v_fov/2.0));
        plane_origin_h = -plane_width/2.0;
        plane_origin_v = plane_height/2.0;
    }
    
    Ray get_ray(const ScreenBuffer &buff, size_t i, size_t j, real h_factor, real v_factor) const {
        real h_pix_size = plane_width / buff.w;
        real v_pix_size = plane_height / buff.h;
        real x = plane_origin_h + i * h_pix_size + h_pix_size * h_factor;
        real y = plane_origin_v - j * v_pix_size + v_pix_size * v_factor;
        Vec3 dir = (Vec3(x,y,1.0) * mat).to_normal();
        return Ray(pos, dir);
    }
};

#endif // camera_H
