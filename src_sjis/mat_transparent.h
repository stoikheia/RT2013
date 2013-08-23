/**
 * 
 * @file mat_transparent.h
 * @author Reiji Tokuda.
 *
 * Interface file of mat_transparent in RT2013 on 2013/08/22.
 * 
 */

#ifndef mat_transparent_H
#define mat_transparent_H

#include "material.h"
#include "radiance.h"

class TransparentMaterial : public Material {
public:
    real refraction;
    ~TransparentMaterial() {};
    
    virtual MatType mat_type() const {return MT_TRANSPARENT;}
};

struct TransparentRadianceContext : public RadianceContext {
    
    size_t step;
    size_t depth;
    real current_refraction;
    real next_refraction;
    real ratio, v_n, v_n2;
    real fresnel_weight;
    real refraction_wight;
    Vec4 radiance;
    const TransparentMaterial *mat;
    std::unique_ptr<const Scene::IntersectionInformation> info;//owner
    
    TransparentRadianceContext(const Material *mat_,
                               const real depth_,
                               std::vector<real> &refraction_stack,
                               std::unique_ptr<const Scene::IntersectionInformation> &&info_)
    :step(0),depth(depth_),
    radiance(0.0),info(std::move(info_))
    {
        assert(mat_->mat_type() == Material::MT_TRANSPARENT);
        mat = static_cast<const TransparentMaterial*>(mat_);
        
        ratio = current_refraction / next_refraction;
        v_n = info->ray.n.dot(info->normal);
        v_n2 = 1.0 - (1.0 - v_n * v_n) * ratio * ratio;
        if(v_n < 0.0) {//enter
            current_refraction = refraction_stack.back();
            refraction_stack.push_back(mat->refraction);
            next_refraction = refraction_stack.back();
        } else {//exit
            current_refraction = refraction_stack.back();
            //assert(refraction_stack.size() == 1);//TODO for internal refrection
            if(1 < refraction_stack.size())refraction_stack.pop_back();
            next_refraction = refraction_stack.back();
        }
        if(4 < depth) {
            fresnel_weight = 0.0;
            refraction_wight = 1.0;
        } else if(v_n2 < 0.0) {//full reflection
            fresnel_weight = 1.0;
            refraction_wight = 0.0;
        } else {
            fresnel_weight = std::min(1.0, fresnel(current_refraction, next_refraction, v_n < 0.0 ? v_n : v_n * -1.0, v_n2));
            refraction_wight = 1.0 - fresnel_weight;
        }
        //assert(0.0 <= refraction_wight);//TODO
    }
    
#if 0
    real fresnel(real m1, real m2, real v_n, real v_n2) {
        real ratio = m1 / m2;
        if(m1 <= m2) {
            return 1.0 - std::pow(ratio * v_n - v_n2 / ratio * v_n + v_n2, 2.0) + pow(ratio * v_n2 - v_n / ratio * v_n2 + v_n, 2.0);
        } else {
            return 1.0 - std::pow(ratio * v_n2 - v_n / ratio * v_n2 + v_n, 2.0) + pow(ratio * v_n - v_n2 / ratio * v_n + v_n2, 2.0);
        }
    }
#else
    real fresnel(real m1, real m2, real v_n, real v_n2) {
        //Schlick formula
        real F0 = std::pow(m1 - m2, 2.0) / std::pow(m1 + m2, 2.0);
        if(m1 <= m2) {
            return F0 + (1.0 - F0) * std::pow(1.0 + v_n, 5.0);
        } else {
            return F0 + (1.0 - F0) * std::pow(1.0 + v_n2, 5.0);
        }
    }
#endif
    
    
    virtual bool step_start(Ray &next_ray)
    {
        if(step < 2) {
            //step1
            if(step == 0 && 0.0 < fresnel_weight) {
                step = 1;
                next_ray.o = info->hitpoint;
                next_ray.n = info->normal * 2.0 * (info->ray.n * -1.0).dot(info->normal) - (info->ray.n * -1.0);
                next_ray.t = type_traits<real>::inf();
                return true;
            }
            //step2
            if(0.0 <= v_n2) {
                step = 2;
                next_ray.o = info->hitpoint;
                if(v_n < 0.0) {//enter
                    Vec3 r_vec = (info->ray.n - info->normal * v_n) * ratio;
                    next_ray.n = info->normal * -sqrt(v_n2) - r_vec;
                } else {//exit
                    Vec3 r_vec = (info->ray.n + info->normal * v_n) * ratio;
                    next_ray.n = info->normal * sqrt(v_n2) - r_vec;
                }//next_ray.n = info->ray.n;//test for noize check
                next_ray.t = type_traits<real>::inf();
                return true;
            }
        }
        step = 3;
        return false;
    }
    virtual bool step_end(const Vec4 &radiance_)
    {
        if(step == 1) {
            radiance = radiance_ * fresnel_weight;
            return true;
        } else if(step == 2) {
            radiance = radiance + radiance_ * refraction_wight;
            return true;
        } else {
            return false;
        }
    }
    virtual Vec4 result() {
        return radiance;
    }
};


#endif // mat_transparent_H
