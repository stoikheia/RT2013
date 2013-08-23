/**
 * 
 * @file mat_reflection.h
 * @author Reiji Tokuda.
 *
 * Interface file of reflection_mat in RT2013 on 2013/08/22.
 * 
 */

#ifndef mat_reflection_H
#define mat_reflection_H

#include "material.h"
#include "radiance.h"

class ReflectionMaterial : public Material {
public:
    real reflection;
    ~ReflectionMaterial() {};
    
    virtual MatType mat_type() const {return MT_REFLECTION;}
};

struct ReflectionRadianceContext : public RadianceContext {
    
    Environment &env;
    real russsion_roulette_probabillity;
    size_t step;
    size_t depth;
    Vec4 radiance;
    const ReflectionMaterial *mat;
    std::unique_ptr<const Scene::IntersectionInformation> info;//owner
    
    ReflectionRadianceContext(Environment &env_,
                              const Material *mat_,
                              size_t depth_,
                              std::unique_ptr<const Scene::IntersectionInformation> &&info_)
    :env(env_),step(0),depth(depth_),
    radiance(0.0),info(std::move(info_))
    {
        assert(mat_->mat_type() == Material::MT_REFLECTION);
        mat = static_cast<const ReflectionMaterial*>(mat_);
        russsion_roulette_probabillity = mat->reflection;
        if(10 < depth) {
            russsion_roulette_probabillity *= (1.0 / (depth - 9));
        }
    }
    
    virtual bool step_start(Ray &next_ray) {
        if(step == 0) {
            if(env.rand01() < russsion_roulette_probabillity) {
                next_ray.o = info->hitpoint;
                next_ray.n = info->normal * 2.0 * (info->ray.n * -1.0).dot(info->normal) - (info->ray.n * -1.0);
                next_ray.t = type_traits<real>::inf();
                step++;
                return true;
            }
        }
        return false;
    }
    virtual bool step_end(const Vec4 &radiance_) {
        radiance = radiance_;
        return false;
    }
    virtual Vec4 result() {
        return radiance;
    }
};


#endif // mat_reflection_H
