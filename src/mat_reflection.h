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

struct RefrectionRadianceContext : public RadianceContext {
    
    Vec4 radiance;
    const ReflectionMaterial *mat;
    const Scene::IntersectionInformation &info;
    
    RefrectionRadianceContext(const Material *mat_,
                              const Scene::IntersectionInformation &info_)
    :radiance(0.0),info(info_) {
        assert(mat_->mat_type() == Material::MT_REFLECTION);
        mat = (ReflectionMaterial*)mat_;
    }
    
    virtual bool step_start(Ray &next_ray) {
        //TODO
        return false;
    }
    virtual bool step_end(const Vec4 &radiance) {
        //TODO
        return false;
    }
    virtual Vec4 result() {
        //TODO
        return radiance;
    }
};


#endif // mat_reflection_H
