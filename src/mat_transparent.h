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
    
    Vec4 radiance;
    const TransparentMaterial *mat;
    const Scene::IntersectionInformation &info;
    
    TransparentRadianceContext(const Material *mat_,
                              const Scene::IntersectionInformation &info_)
    :radiance(0.0),info(info_) {
        assert(mat_->mat_type() == Material::MT_TRANSPARENT);;
        mat = (TransparentMaterial*)mat_;
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


#endif // mat_transparent_H
