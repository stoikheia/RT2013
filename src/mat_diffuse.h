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
    
    Vec4 radiance;
    const DiffuseMaterial *mat;
    const Scene &scene;
    std::unique_ptr<const Scene::IntersectionInformation> info;//owner
    
    DiffuseRadianceContext(const Material *mat_,
                           const Scene &scene_,
                           std::unique_ptr<const Scene::IntersectionInformation> &&info_)
    :radiance(0.0),scene(scene_),info(std::move(info_)) {
        assert(mat_->mat_type() == Material::MT_DIFFUSE);
        mat = static_cast<const DiffuseMaterial*>(mat_);
    }
    virtual ~DiffuseRadianceContext() {}
    
    virtual bool step_start(Ray &next_ray) {
        return false;
    }
    virtual bool step_end(const Vec4 &radiance) {
        return false;
    }
    virtual Vec4 result() {
        for(auto it = scene.lights.begin(); it != scene.lights.end(); ++it) {
            Ray p2l = Ray::create_start_end(info->hitpoint, *it);
            
            real light_angle = p2l.n.dot(info->normal);
            Vec4 tmp_radiance(0.0);
            if(0 <= light_angle) {
                //diffuse
                tmp_radiance = mat->diffuse * light_angle;
                
                //specular
                Vec3 ref_light = info->normal * 2.0 * p2l.n.dot(info->normal) - p2l.n;
                real norm_facotr = 1;//(50.0+2.0)/(2.0*M_PI);
                real specular_power = norm_facotr * pow(std::min(0.0, info->ray.n.dot(ref_light)), 50.0);
                tmp_radiance += mat->specular * specular_power;
                
                //shadow
                if(scene.is_shadow_point(p2l, *info)) {
                    radiance += tmp_radiance * 0.5;
                } else {
                    radiance += tmp_radiance;
                }
            }
        }
        return radiance;
    }
};


#endif // mat_diffuse_H
