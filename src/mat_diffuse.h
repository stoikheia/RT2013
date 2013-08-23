/**
 * 
 * @file mat_diffuse.h
 * @author Reiji Tokuda.
 *
 * Interface file of diffuse in RT2013 on 2013/08/22.
 * 
 */

#ifndef mat_diffuse_H
#define mat_diffuse_H

#include "material.h"
#include "radiance.h"

#include <memory>


class DiffuseMaterial : public Material {
    
public:
    Vec4 diffuse;
    Vec4 specular;
    Vec4 emission;
    
    DiffuseMaterial() = default;
    DiffuseMaterial(const DiffuseMaterial &m) = default;
    DiffuseMaterial(DiffuseMaterial &&) = default;
    virtual ~DiffuseMaterial() {};
    DiffuseMaterial& operator=(const DiffuseMaterial &) = default;
    DiffuseMaterial& operator=(DiffuseMaterial &&) = default;
    
    DiffuseMaterial(real val)
    :diffuse(val),specular(val),emission(val) {}
    
    virtual MatType mat_type() const {return MT_DIFFUSE;}
};


struct DiffuseRadianceContext : public RadianceContext {
    
    Environment &env;
    real russsion_roulette_probabillity;
    Vec4 weight;
    size_t step;
    bool step_flag;
    Vec3 step_direction;
    Vec4 radiance;
    const DiffuseMaterial *mat;
    const Scene &scene;
    std::unique_ptr<const Scene::IntersectionInformation> info;//owner
    
    DiffuseRadianceContext(Environment &env_,
                           const Material *mat_,
                           const Scene &scene_,
                           std::unique_ptr<const Scene::IntersectionInformation> &&info_)
    :env(env_),radiance(0.0),step(0),step_flag(false),scene(scene_),info(std::move(info_))
    {
        assert(mat_->mat_type() == Material::MT_DIFFUSE);
        mat = static_cast<const DiffuseMaterial*>(mat_);
        
        russsion_roulette_probabillity = rgb_luminance(mat->diffuse);
        weight = mat->diffuse * russsion_roulette_probabillity;
    }
    virtual ~DiffuseRadianceContext() {}
    
    real rgb_luminance(const Vec4 &color)
    {
        //return 0.298912 * color.r() + 0.586611 * color.g() + 0.114478 * color.b();//human test
        return std::max(color.r(), std::max(color.g(), color.g()));
    }
    
    void create_montecarlo_direction(Vec3 &n)
    {
        Vec3 w(info->normal.dot(info->ray.n)), u, v;//from edupt
        if(std::abs(w.x()) > 1.0e-8) {
            u = Vec3(0.0, 1.0, 0.0).cross(w).to_normal();
        } else {
            u = Vec3(1.0, 0.0, 0.0).cross(w).to_normal();
        }
        v = w.cross(u);
        const real theta = 2.0 * M_PI * env.rand01();
        const real phi = env.rand01();
        const real dist = std::sqrt(phi);
        n = u * cos(theta) * dist + v * sin(theta) * dist + w * sqrt(1.0 - dist);
        n.to_normal();
    }
    
    virtual bool step_start(Ray &next_ray) {
        if(step == 0) {
            step = 1;
            if(env.rand01() < russsion_roulette_probabillity) {
                next_ray.o = info->hitpoint;
                create_montecarlo_direction(next_ray.n);
                next_ray.t = type_traits<real>::inf();
                step_flag = true;
                return true;
            }
        } 
        return false;
    }
    virtual bool step_end(const Vec4 &radiance_) {
        if(step == 1) {
            if(step_flag) {
                for(size_t i = 0; i < 4; ++i) {
                    radiance.e[i] = mat->emission.e[i] + radiance_.e[i] * weight.e[i];
                }
                return true;
            } else {
                radiance = mat->emission;
                return true;
            }
        }
        return false;
    }
    virtual Vec4 result() {
        return radiance;
    }
};


#endif // mat_diffuse_H
